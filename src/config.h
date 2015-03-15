
#pragma once

#include <string>
#include <SDL.h>

#define NUM_COLORS 4

//use these keys with get_color()
enum Color {
	BACKGROUND = 0,
	FILL,
	HIGHLIGHT,
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

		bool fullscreen;
		bool resizable;
		int window_x;
		int window_y;
		int window_w;
		int window_h;

		SDL_Color colors[NUM_COLORS];

		std::string font_path;
		int font_size;

		unsigned int max_suggest;

		const int CLI_height = 20;
		const int CLI_padding = 4;

		//a few handy accessors
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		SDL_Color get_color(Color c);
};

//declared and instantiated in collector.cpp
extern Config* config;
