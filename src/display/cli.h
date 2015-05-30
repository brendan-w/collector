
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# cli.h                                                                #
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

#include "SDL.h"

#include <text.h>
#include <filestore/types.h>
#include <display/displayobject.h>



class CLI : public DisplayObject
{
	public:
		CLI(State* s);
		~CLI();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		void on_state_change();
		void fill_selector(Selector* selector);

	private:

		DisplayObject* tags;
		DisplayObject* command;

		//the CLI's mode
		DisplayObject* current;

		//standard readouts
		Text* prompt;
		Text* totals;
		Text* includes;
		Text* excludes;

		void carry_dirt();
		void cycle_mode();
		void set_mode(DisplayObject* m);
};
