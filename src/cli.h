
#pragma once

#include <string>
#include <vector>
#include "text.h"

class CLI
{
	public:
		CLI();
		~CLI();
		void handle_key(SDL_KeyboardEvent &e);
		void handle_text(SDL_TextInputEvent &e);
		void render();
	private:
		std::vector<Text*> tags;
		unsigned int current;

		void new_tag();
		void destroy_tags();
		void delete_tag();
		Text* current_tag();

		void send_quit();
		void send_new_selector();
};
