
#pragma once

#include <string>
#include <vector>
#include "text.h"

const int CLI_height = 20;
const int CLI_padding = 4;

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
};
