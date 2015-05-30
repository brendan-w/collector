
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# file.h                                                               #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/

#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <SDL.h>

#include <thumbnail.h>


//raw string tag collections
typedef std::vector<std::string> TagVector;
typedef std::unordered_set<std::string> TagSet;


//SDL_Point =  8 bytes
//this = 4 bytes
typedef struct {
	uint16_t x;
	uint16_t y;
} File_Point;


//forward declare
class TagEntry;

//TagEntry collections
typedef std::vector<TagEntry*> EntryVector;
typedef std::unordered_set<TagEntry*> EntrySet;


class File
{
	public:
		File(std::string path);
		~File();

		//functions

		//opens the file in its default application
		//the filename is optional in case the symlink needs to be opened
		void open(std::string filepath);

		//tag operations
		std::string get_exemplar_tag();
		TagSet compute_tags();
		bool has_tag(TagEntry* t);
		void add_tag(TagEntry* t);
		void remove_tag(TagEntry* t);

		size_t get_size();
		std::string get_path();
		std::string get_full_path();
		std::string get_link_path();

		void unload();
		Thumbnail* get_thumb();

		//properties

		File_Point grid_pos;
		File_Point thumb_pos;
		EntrySet tags;

	private:
		//the path relative to the curerent working directory
		std::string path;

		Thumbnail* thumb;

		TagSet split_tags(std::string p);
		bool move(std::string dest);
};


//FileVector
typedef std::vector<File*> FileVector;
typedef FileVector::iterator FileVector_it;

//FileSet
typedef std::unordered_set<File*> FileSet;
typedef FileSet::iterator FileSet_it;


class TagEntry
{
	public:
		std::string tag;
		FileSet files;
};
