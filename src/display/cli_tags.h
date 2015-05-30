
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# cli_tags.h                                                           #
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

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>


class CLI_Tag
{
	public:
		CLI_Tag();
		~CLI_Tag();

		//helpers, merely to make the code prettier
		std::string get_t() { return text->get_text(); }
		std::string get_c() { return completion->get_text(); }
		void set_t(std::string str) { text->set_text(str); }
		void set_c(std::string str) { completion->set_text(str); }
		size_t len_t() { return text->get_text().length(); }
		size_t len_c() { return completion->get_text().length(); }

		//helpers to account for control characters
		Tag get_tag();
		Set_Operation get_op();

		Text* text;       //the users actual input
		Text* completion; //the generated auto completion
};


class CLI_Tags : public DisplayObject
{
	public:
		CLI_Tags(State* s);
		~CLI_Tags();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		void fill_selector(Selector* selector);

	private:

		std::vector<CLI_Tag*> tags;
		size_t current_index;

		void render_tags(int x, const int y);
		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		void send_tag_info();
		void auto_complete();
		CLI_Tag* current_tag();
};
