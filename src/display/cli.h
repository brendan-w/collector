
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>



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

		DisplayObject* tags;
		DisplayObject* command;

		//the CLI's mode
		DisplayObject* current;

		//standard readouts
		Text* prompt;
		Text* totals;
		Text* includes;
		Text* excludes;

		void carry_dirt();
		void cycle_mode();
		void set_mode(DisplayObject* m);
};
