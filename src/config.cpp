
#include <string.h>
#include <SDL.h>
#include "config.h"


Config::Config()
{
	font_path = "../assets/MonoLiberation.ttf";
	font_size = 12;
	fullscreen = false;
	resizable = true;
	window_x = SDL_WINDOWPOS_UNDEFINED;
	window_y = SDL_WINDOWPOS_UNDEFINED;
	window_w = 640;
	window_h = 480;

}

Config::~Config()
{

}


std::string Config::get_font_path()
{
	return font_path;
}

int Config::get_font_size()
{
	return font_size;
}

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


int Config::get_window_x()
{
	return window_x;
}

int Config::get_window_y()
{
	return window_y;
}

int Config::get_window_w()
{
	return window_w;
}

int Config::get_window_h()
{
	return window_h;
}
