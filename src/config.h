
#pragma once

#include <string>
#include <SDL.h>

#define NUM_COLORS 10

//use these keys with get_color()
enum Color
{
	BACKGROUND = 0,
	OVERLAY,
	HIGHLIGHT,
	CLI_LIGHT,
	CLI_DARK,
	CLI_ERROR,
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

		std::string bin_path;
		std::string cwd_path;
		std::string export_path;

		std::string find_cmd;
		std::string mime_cmd;
		std::string open_cmd;
		std::string tag_delim;
		std::string font_path;

		bool fullscreen;
		bool resizable;
		SDL_Rect window;

		SDL_Color colors[NUM_COLORS];

		size_t scroll_speed;
		int font_size;
		int file_size;
		int file_thumb_size;
		int file_padding;
		int CLI_height;
		int CLI_padding;

		//a few handy accessors
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		SDL_Color get_color(Color c);
};
