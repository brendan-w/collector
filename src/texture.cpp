
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# texture.cpp                                                          #
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


#include <string>

#include <SDL.h>

#include <texture.h>
#include <collector.h>
#include <SDL_utils.h>


Texture::Texture()
{
	texture = NULL;
	w = 0;
	h = 0;
}


Texture::~Texture()
{
	free();
}


void Texture::free()
{
	if(texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		w = 0;
		h = 0;
	}
}


bool Texture::load_surface(SDL_Surface* surface)
{
	free();

	texture = SDL_CreateTextureFromSurface(sdl->get_renderer(), surface);

	if(texture == NULL)
	{
		print_SDL_error("Failed to create texture from surface");
		return false;
	}

	w = surface->w;
	h = surface->h;

	return true;
}

void Texture::render(int _x, int _y)
{
	render(_x, _y, w, h);
}

void Texture::render(SDL_Rect *r)
{
	render(r->x, r->y, r->w, r->h);
}

void Texture::render(int _x, int _y, int _w, int _h)
{
	if((texture != NULL) && (w > 0) && (h > 0))
	{
		SDL_Rect dest = { _x, _y, _w, _h };
		if(sdl->rect_in_viewport(dest))
			SDL_RenderCopy(sdl->get_renderer(), texture, NULL, &dest);
	}
}

int Texture::width()
{
	return w;
}

int Texture::height()
{
	return h;
}
