
#include <string.h>
#include <SDL.h>
#include "config.h"


Config::Config()
{	
	fullscreen = false;
	resizable = true;
	window_x = SDL_WINDOWPOS_UNDEFINED;
	window_y = SDL_WINDOWPOS_UNDEFINED;
	window_w = 640;
	window_h = 480;

	font_path = "../assets/MonoLiberation.ttf";
	font_size = 12;

	background_color = { 0,   0,   0,   255};
	fill_color       = { 40,  40,  40,  255};
	highlight_color  = { 18,  53,  70,  255};
	cli_text_color   = { 255, 255, 255, 255};
}

Config::~Config()
{

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

int Config::get_window_x() { return window_x; }
int Config::get_window_y() { return window_y; }
int Config::get_window_w() { return window_w; }
int Config::get_window_h() { return window_h; }

std::string Config::get_font_path()      { return font_path; }
int Config::get_font_size()              { return font_size; }

SDL_Color Config::get_background_color() { return background_color; }
SDL_Color Config::get_fill_color()       { return fill_color; }
SDL_Color Config::get_highlight_color()  { return highlight_color; }
SDL_Color Config::get_cli_text_color()   { return cli_text_color; }
