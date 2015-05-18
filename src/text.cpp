
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# text.cpp                                                             #
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

#include <iostream>

#include <SDL_ttf.h>

#include <collector.h>
#include <text.h>
#include <SDL_utils.h> //colors_equal()


Text::Text(std::string t, SDL_Color c)
{
	color = c;
	set_text(t);
}


//get/set text


std::string Text::get_text()
{
	return text;
}

void Text::set_text(std::string s)
{
	text = s;

	//if the texture needs updating
	if(text != current_text)
		load_text();
}


//get/set color


SDL_Color Text::get_color()
{
	return color;
}

void Text::set_color(SDL_Color c)
{
	if(!colors_equal(color, c))
	{
		color = c;
		load_text();
	}
}


void Text::load_text()
{
	if(text.length() > 0)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(sdl->get_font(), text.c_str(), color);

		if(surface == NULL)
		{
			print_TTF_error("Failed to render text to surface");
			return;
		}

		load_surface(surface);
		SDL_FreeSurface(surface);
	}
	else
	{
		//prevent rendering of null strings
		free();
	}

	current_text = text;
}
