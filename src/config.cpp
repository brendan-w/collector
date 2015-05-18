
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# config.cpp                                                           #
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

#include <string.h>

#include <SDL.h>

#include <utils.h> //PATH_SEP
#include <config.h>



Config::Config(std::string cwd)
{
	/*
		Set default configuration before reading config file
	*/

	//use the executable location to find the assets
	bin_path = std::string(SDL_GetBasePath());

	cwd_path = cwd;

	//the filestore is expecting this to be WITHOUT the trailing slash
	if(cwd_path.back() == PATH_SEP)
		cwd_path.pop_back();

	//the directory where symlinks will be placed
	//(representing the selection)
	export_path = "/tmp/collector";

	//the font file
	font_path = path_join(bin_path, "../assets/MonoLiberation.ttf");

	//lists all items in the current directory and below
	//only returns files
	//excludes dot-files and dot-folders
	//excludes executables where o=x
	find_cmd = "find " + cwd_path + " -type f -path \"*\" ! -path \"*/.*\" ! -perm -o=x";

	//command returning the mime-type of the requested file
	mime_cmd = "xdg-mime query filetype ";

	//command to open arbitrary files
	open_cmd = "exo-open ";

	//the delimeters by which paths will be split
	tag_delim = " ._-+&%%()[]{}";
	tag_delim += PATH_SEP;

	//used when adding tags to filenames
	default_tag_delim = "_";
	
	fullscreen = false;
	resizable = true;
	window = {
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1000,
		650
	};

	colors[BACKGROUND] = { 50,  50,  50,  255 };
	colors[OVERLAY]    = { 0,   0,   0,   255 };
	colors[HIGHLIGHT]  = { 24,  71,  93,  255 };
	colors[CLI_LIGHT]  = { 255, 255, 255, 255 };
	colors[CLI_DARK]   = { 128, 128, 128, 255 };
	colors[CLI_ERROR]  = { 255, 70,  70,  255 };

	colors[FILE_NEUTRAL]        = { 75,  75,  75,  255 };
	colors[FILE_NEUTRAL_HOVER]  = { 95,  95,  95,  255 };
	colors[FILE_SELECTED]       = { 48,  141, 186, 255 };
	colors[FILE_SELECTED_HOVER] = { 68,  161, 206, 255 };
	colors[FILE_INCLUDED]       = { 70,  255, 70,  255 };
	colors[FILE_INCLUDED_HOVER] = { 100, 255, 100, 255 };
	colors[FILE_EXCLUDED]       = { 255, 70,  70,  255 };
	colors[FILE_EXCLUDED_HOVER] = { 255, 100, 100, 255 };

	colors[THUMB_INCLUDED]      = { 70,  255, 70,  100 };

	scroll_speed = 120;
	grid_tag_spacing = 20; //number of columns before the next tag
	font_size = 12;
	file_size = 9;
	file_thumb_size = 149;
	file_padding = 1;
	CLI_height = 20;
	CLI_padding = 4;
}

Config::~Config()
{

}

/*
	Getters
*/

Uint32 Config::get_window_flags()
{
	Uint32 flags = 0;
	flags |= SDL_WINDOW_SHOWN;
	flags |= (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	flags |= (resizable ? SDL_WINDOW_RESIZABLE : 0);
	return flags;
}

Uint32 Config::get_render_flags()
{
	return SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

SDL_Color Config::get_color(Color c) { return colors[c]; }
