
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# config.h                                                             #
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
#include <SDL.h>

#define NUM_COLORS 15

//use these keys with get_color()
enum Color
{
	BACKGROUND = 0,
	OVERLAY,
	HIGHLIGHT,
	CLI_LIGHT,
	CLI_DARK,
	CLI_ERROR,
	FILE_NEUTRAL,
	FILE_NEUTRAL_HOVER,
	FILE_SELECTED,
	FILE_SELECTED_HOVER,
	FILE_INCLUDED,
	FILE_INCLUDED_HOVER,
	FILE_EXCLUDED,
	FILE_EXCLUDED_HOVER,
	THUMB_INCLUDED
};

class Config
{
	public:
		Config(std::string cwd);
		~Config();

		std::string bin_path;
		std::string cwd_path;
		std::string export_path;
		std::string font_path;

		std::string find_cmd;
		std::string mime_cmd;
		std::string open_cmd;
		std::string tag_delim;
		std::string default_tag_delim;

		bool fullscreen;
		bool resizable;
		SDL_Rect window;

		SDL_Color colors[NUM_COLORS];

		size_t scroll_speed;
		size_t grid_tag_spacing;
		int font_size;
		int file_size;
		int file_thumb_size;
		int file_padding;
		int CLI_height;
		int CLI_padding;

		//a few handy accessors
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		SDL_Color get_color(Color c);
};
