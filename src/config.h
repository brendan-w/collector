
#pragma once

#include <string>
#include <SDL.h>

#define NUM_COLORS 8

//use these keys with get_color()
enum Color
{
	BACKGROUND = 0,
	OVERLAY,
	CLI_HIGHLIGHT,
	CLI_TEXT,
	FILE_NEUTRAL,
	FILE_SELECTED,
	FILE_NEUTRAL_HOVER,
	FILE_SELECTED_HOVER
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
		SDL_Rect window;

		SDL_Color colors[NUM_COLORS];

		int font_size;
		size_t max_suggest;
		size_t scroll_speed;
		int file_size;
		int file_thumb_size;
		int file_padding;
		int CLI_height;
		int CLI_padding;

		//a few handy accessors
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		SDL_Color get_color(Color c);
		SDL_Rect get_window_rect();
};

//declared and instantiated in collector.cpp
extern Config* config;

//just to make the code cleaner
#define WINDOW_W (config->window.w)
#define WINDOW_H (config->window.h)
#define CLI_H (config->CLI_height)
#define CLI_PAD (config->CLI_padding)
#define FILE_SIZE (config->file_size)
#define FILE_THUMB_SIZE (config->file_thumb_size)
#define FILE_PAD (config->file_padding)
#define FILE_OFFSET (FILE_SIZE + FILE_PAD)
#define FILE_THUMB_OFFSET (FILE_THUMB_SIZE + FILE_PAD)
