
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# state.h                                                              #
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

#include <filestore/types.h>

/*
	A place to save state shared between DisplayObjects
*/

class State
{
	public:
		State();
		~State();

		Selection* selection = NULL;    //the current file selection returned from the filestore
		File* file_under_mouse = NULL;  //pointer to the file currently under the mouse
		file_map_bool inexclude;        //manual include/exclude table
		bool should_autoscroll = false; //whether or not the most recent selection should be autoscrolled
		bool key_ctrl = false;         //whether a shift key is depressed

		void replace_selection(Selection* s);
		void fill_selector(Selector* s);
		void inexclude_file(File* f, bool include);
		void toggle_inexclude(File* f, bool include);
		bool inexclude_has(File* f);
		void clear_includes();
		void clear_excludes();

	private:
		void remove_inexclude_by_val(bool val);
};
