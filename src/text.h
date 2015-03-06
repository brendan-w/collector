
#pragma once

#include <SDL.h>
#include <string>
#include "texture.h"
#include "text.h"



class Text: public Texture
{
	public:
		Text(std::string t, SDL_Color c);
		void render(int x, int y);

		void set_text(std::string s);
		std::string get_text();

		void set_color(SDL_Color c);
		SDL_Color get_color();

	private:
		void load_text();

		std::string text;
		std::string current_text;
		SDL_Color color;
};
