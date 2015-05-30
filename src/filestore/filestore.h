
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# filestore.h                                                          #
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
#include <unordered_map>

#include <filestore/types.h>




class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		Selection* select(Selector* selector);
		void operation(Operation* operation);

	private:
		file_vector files;
		tag_map tags;

		bool has_tag(const std::string & tag);
		void insert_file(File* file);

		void add_tag(Selection* const selection, const std::string & tag);
		void remove_tag(Selection* const selection, const std::string & tag);
};
