
#pragma once

#include <string>

class CLI
{
	public:
		CLI();
		~CLI();
		void handle_key(SDL_KeyboardEvent &e);
		void handle_text(SDL_TextInputEvent &e);
		void render();
		void set_dirty();
	private:
		bool dirty;
		std::string text;

		void send_quit();
};
