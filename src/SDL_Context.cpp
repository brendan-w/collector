
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# SDL_Context.cpp                                                      #
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


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <collector.h>
#include <SDL_Context.h>
#include <SDL_utils.h>


#define NUM_USER_EVENTS 5

Uint32 SELECTOR = 0;
Uint32 SELECTION = 0;
Uint32 OPERATION = 0;
Uint32 RENDER_THUMBS = 0;
Uint32 STATE_CHANGE = 0;


SDL_Context::SDL_Context()
{
	/*
		SDL2
	*/

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		print_SDL_error("SDL_Init Error");
		success = false;
		return;
	}

	std::string title = "Collector - " + config->cwd_path;
	window = SDL_CreateWindow(title.c_str(),
                              config->window.x,
                              config->window.y,
                              config->window.w,
                              config->window.h,
                              config->get_window_flags());
	if(window == NULL)
	{
		print_SDL_error("SDL_CreateWindow Error");
		success = false;
		return;
	}

	renderer = SDL_CreateRenderer(window,
                                  -1,
                                  config->get_render_flags());
	if(renderer == NULL)
	{
		print_SDL_error("SDL_CreateRenderer Error");
		success = false;
		return;
	}

	//clear the screen with the background color
	set_color(BACKGROUND);
	SDL_RenderClear(renderer);
	present();

	SDL_StartTextInput();

	if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		print_SDL_error("SDL_SetRenderDrawBlendMode Error");
		success = false;
		return;
	}

	/*
		SDL2_ttf
	*/

	if(TTF_Init() != 0)
	{
		print_TTF_error("SDL_ttf could not initialize");
		success = false;
		return;
	}

	font = TTF_OpenFont(config->font_path.c_str(),
						config->font_size);
	if(font == NULL)
	{
		print_TTF_error("Failed to load font: " + config->font_path);
		success = false;
		return;
	}

	/*
		SDL2_image
	*/

	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

	if((IMG_Init(flags) & flags) != flags)
	{
		print_IMG_error("SDL_image could not initialize");
		success = false;
		return;
	}

	/*
		Event Registration
	*/

	Uint32 begin = SDL_RegisterEvents(NUM_USER_EVENTS);
	if(begin == ((Uint32) -1 ))
	{
		print_message("Failed to register custom events");
		success = false;
		return;
	}


	SELECTOR      = begin;
	SELECTION     = begin + 1;
	OPERATION     = begin + 2;
	RENDER_THUMBS = begin + 3;
	STATE_CHANGE  = begin + 4;
}


SDL_Context::~SDL_Context()
{
	/*
		SDL2_image
	*/

	IMG_Quit();

	/*
		SDL2_ttf
	*/

	TTF_CloseFont(font);
	TTF_Quit();

	/*
		SDL2
	*/

	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void SDL_Context::submit(Uint32 type, void* data1 /*=NULL*/, void* data2 /*=NULL*/)
{
	SDL_Event e;
	SDL_zero(e);

	e.type = type;
	e.user.data1 = data1;
	e.user.data2 = data2;

	//try to push onto SDL event queue
	int r = SDL_PushEvent(&e);

	if(r < 0)
		print_SDL_error("SDL_PushEvent() failed");
	else if(r == 0)
		print_message("SDL_PushEvent() was filtered");
}

void SDL_Context::quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}

void SDL_Context::present()
{
	SDL_RenderPresent(renderer);
}

void SDL_Context::set_color(Color c)
{
	set_color(config->get_color(c));
}

void SDL_Context::set_color(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
}

SDL_Point SDL_Context::window_size()
{
	SDL_Point p;
	SDL_GetWindowSize(window, &p.x, &p.y);
	return p;
}

void SDL_Context::fill_rect(SDL_Rect &rect)
{
	SDL_RenderFillRect(renderer, &rect);
}

void SDL_Context::draw_line(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void SDL_Context::set_viewport(SDL_Rect &rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void SDL_Context::reset_viewport()
{
	SDL_Point w = window_size();
	SDL_Rect r = { 0, 0, w.x, w.y };
	set_viewport(r);
}

SDL_Rect SDL_Context::get_viewport()
{
	SDL_Rect rect;
	SDL_RenderGetViewport(renderer, &rect);
	rect.x = 0;
	rect.y = 0;
	return rect;
}

bool SDL_Context::rect_in_viewport(SDL_Rect &rect)
{
	SDL_Rect viewport = get_viewport();
	viewport.x = 0;
	viewport.y = 0;
	return SDL_HasIntersection(&viewport, &rect);
}

