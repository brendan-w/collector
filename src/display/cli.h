
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <display/displayobject.h>
#include <events/selector.h>
#include <text.h>

class CLI : public DisplayObject
{
	public:
		CLI();
		~CLI();

		void render();
		void layout();

		bool on_key(SDL_KeyboardEvent &e);
		bool on_text(SDL_TextInputEvent &e);

		void fill_selector(Selector* selector);
		void read_selection(Selection* selection);

	private:
		std::vector<Text*> tags;
		unsigned int current;

		Text* totals;

		void render_tags();

		void new_tag();
		void destroy_tags();
		bool delete_tag();
		bool backspace();
		Text* current_tag();
};
