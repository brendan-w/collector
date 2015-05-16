
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

		Text* text;       //the users actual input
		Text* completion; //the generated auto completion
};


class CLI : public DisplayObject
{
	public:
		CLI(State* s);
		~CLI();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		void on_state_change();
		void fill_selector(Selector* selector);

	private:
		std::vector<Tag*> tags;
		unsigned int current_index;

		Text* totals;
		Text* includes;
		Text* excludes;

		void render_tags();
		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		void send_tag_info();
		void auto_complete();
		Tag* current_tag();
};
