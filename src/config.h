
#pragma once

#include <string>

//use these keys with get_color()

#define NUM_COLORS 4

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
		
		Uint32 get_window_flags();
		Uint32 get_render_flags();
		int get_window_x();
		int get_window_y();
		int get_window_w();
		int get_window_h();

		std::string get_font_path();
		int get_font_size();

		SDL_Color get_color(Color c);

		std::string get_cwd();
		std::string get_find_cmd();
		std::string get_tag_delim();

	private:
		bool fullscreen;
		bool resizable;
		int window_x;
		int window_y;
		int window_w;
		int window_h;

		std::string font_path;
		int font_size;

		SDL_Color colors[NUM_COLORS];

		std::string cwd;
		std::string find_cmd;
		std::string tag_delim;
};
