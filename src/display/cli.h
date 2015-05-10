
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <text.h>


class CLI : public DisplayObject
{
	public:
		CLI(Selection** s);
		~CLI();

		void render();

		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		void on_tag_info(Tag_Info* completion);
		void fill_selector(Selector* selector);

	private:
		std::vector<Text*> tags;
		unsigned int current_index;

		Text* totals;

		void render_tags();
		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		void send_tag_info();
		Text* current_tag();
};
