

#pragma once


#include <string>
#include <SDL_ttf.h>


class CLI
{
	public:
		CLI();
		~CLI();
		void handle_key(SDL_KeyboardEvent &e);
		void handle_text(SDL_TextInputEvent &e);
		void render();
	private:
		bool dirty;
		std::string text;
};
