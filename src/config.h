
#pragma once

#include <string>

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

		SDL_Color get_background_color();
		SDL_Color get_fill_color();
		SDL_Color get_highlight_color();
		SDL_Color get_cli_text_color();

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

		SDL_Color background_color;
		SDL_Color fill_color;
		SDL_Color highlight_color;
		SDL_Color cli_text_color;

		std::string cwd;
		std::string find_cmd;
		std::string tag_delim;
};
