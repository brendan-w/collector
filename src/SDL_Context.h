
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# SDL_Context.h                                                        #
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

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <config.h>


class SDL_Context
{
	public:
		SDL_Context();
		~SDL_Context();

		SDL_Window* get_window() { return window;}
		SDL_Renderer* get_renderer() { return renderer; }
		TTF_Font* get_font() { return font; }

		void submit(Uint32 type, void* data1=NULL, void* data2=NULL);
		void quit();

		void present();

		void set_color(Color c);
		void set_color(SDL_Color color);
		void fill_rect(SDL_Rect &rect);
		void draw_line(int x1, int y1, int x2, int y2);

		SDL_Point window_size();
		void set_viewport(SDL_Rect &rect);
		SDL_Rect get_viewport();
		void reset_viewport();
		bool rect_in_viewport(SDL_Rect &rect);

		bool succeeded() { return success; }

	private:
		bool success = true;

		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		TTF_Font* font = NULL;
};
