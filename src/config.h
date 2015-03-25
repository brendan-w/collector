
#pragma once

#include <string>
#include <SDL.h>

#define NUM_COLORS 6

//use these keys with get_color()
enum Color
{
	BACKGROUND = 0,
	FILL,
	HIGHLIGHT,
	OVERLAY,
	CLI_HIGHLIGHT,
	CLI_TEXT
};

class Config
{
	public:
		Config();
		~Config();

		std::string find_cmd;
		std::string bin_path;
		std::string cwd_path;
		std::string tag_delim;
		std::string font_path;

		bool fullscreen;
		bool resizable;
		int window_x;
		int window_y;
		int window_w;
		int window_h;

		SDL_Color colors[NUM_COLORS];

		int font_size;
		size_t max_suggest;
		size_t scroll_speed;
		int file_size;
		int file_padding;
		int CLI_height;
		int CLI_padding;

		//a few handy accessors
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		SDL_Color get_color(Color c);
};

//declared and instantiated in collector.cpp
extern Config* config;
