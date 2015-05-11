
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <text.h>


class Tag
{
	public:
		Tag();
		~Tag();


		std::string get_text();
		void set_text(const std::string & t);
		std::string get_completion();
		void set_completion(const std::string & t);

		Text* text;
		Text* completion;
		bool valid;
};


class CLI : public DisplayObject
{
	public:
		CLI(Selection** s);
		~CLI();

		void render();

		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

		void on_selection();
		// void on_tag_info(Tag_Info* completion);
		void fill_selector(Selector* selector);

	private:
		std::vector<Tag*> tags;
		unsigned int current_index;

		Text* totals;

		void render_tags();
		void new_tag();
		void destroy_tags();
		void delete_tag();
		void backspace();
		void send_tag_info();
		void set_current_text(std::string s);
		void auto_complete();
		Tag* current_tag();
};
