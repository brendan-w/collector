
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# selection.h                                                          #
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

#include <utility>

#include <filestore/file.h>


class Selection
{
	public:
		Selection(file_vector* all, file_set fs);
		~Selection();

		//the main IO of collector
		void export_();

		bool has(File* file);
		tag_vector get_subtags() { return subtags_sorted; }

		//the selection
		const file_vector_it begin() { return files_sorted.begin(); }
		const file_vector_it end()   { return files_sorted.end(); }
		size_t size()                { return files_sorted.size(); }
		File* at(size_t i)           { return files_sorted[i]; };

		//all files
		const file_vector_it all_begin() { return all_files->begin(); }
		const file_vector_it all_end()   { return all_files->end(); }
		size_t all_size()                { return all_files->size(); }
		File* all_at(size_t i);

		bool has_subtag(std::string tag);
		std::string auto_complete(std::string partial);

	private:
		
		//set of Files selected by the previous Selector
		file_set files;           //used for file lookup in has()
		file_vector files_sorted; //used for iterating/displaying the sorted files
		
		//the whole file list
		file_vector* const all_files;

		//subtags
		tag_set subtags;
		tag_vector subtags_sorted;

		//don't export the selection more than once
		bool exported;
};
