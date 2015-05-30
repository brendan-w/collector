
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# operation.h                                                          #
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
#include <filestore/types.h>


enum Selection_Operation
{
	ADD_TAG = 0,
	REMOVE_TAG,
	DELETE_FILES
};


class Operation
{
	public:
		Operation(Selection* s);
		~Operation();

		//setters
		void add(Selection_Operation o);
		void add(Selection_Operation o, std::string t);

		//getters
		Selection* get_selection()   { return selection; }
		bool       get_delete()      { return delete_files; }
		TagSet    get_add_tags()    { return add_tags; }
		TagSet    get_remove_tags() { return remove_tags; }
		
		bool is_changing();

	private:
		Selection* const selection;

		//data for all possible actions
		bool delete_files = false;
		TagSet add_tags;
		TagSet remove_tags;
};
