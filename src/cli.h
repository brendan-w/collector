
#pragma once

#include <string>
#include <vector>
#include "text.h"

class CLI
{
	public:
		CLI();
		~CLI();
		void render();
		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);
	private:
		std::vector<Text*> tags;
		unsigned int current;

		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		Text* current_tag();
};
