
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# cli_command.h                                                        #
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
#include <deque>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>



class CLI_Command : public DisplayObject
{
	public:
		CLI_Command(State* s);
		~CLI_Command();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

	private:

		//buffer, to save the users text while scrolling through history
		std::string cmd = "";

		//the current location in history when using the up/down keys
		//-1 = blank
		//0  = most recent
		//etc...
		int history_index = -1;
		std::deque<std::string> history;

		size_t cursor_index = 0;

		Text* command;
		Text* completion;

		void execute();
		void parse_command(Operation* op, std::string & cmd);
		void post_execute();
		void backspace();
		void update_completion();
};
