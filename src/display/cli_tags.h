
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>


class Tag
{
	public:
		Tag();
		~Tag();

		//helpers, merely to make the code prettier
		std::string get_t() { return text->get_text(); }
		std::string get_c() { return completion->get_text(); }
		void set_t(std::string str) { text->set_text(str); }
		void set_c(std::string str) { completion->set_text(str); }
		size_t len_t() { return text->get_text().length(); }
		size_t len_c() { return completion->get_text().length(); }

		//helpers to account for control characters
		std::string get_tag();
		Set_Operation get_op();

		Text* text;       //the users actual input
		Text* completion; //the generated auto completion
};


class CLI_Tags : public DisplayObject
{
	public:
		CLI_Tags(State* s);
		~CLI_Tags();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		void fill_selector(Selector* selector);

	private:

		std::vector<Tag*> tags;
		size_t current_index;

		void render_tags(int x, const int y);
		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		void send_tag_info();
		void auto_complete();
		Tag* current_tag();
};
