
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>



class CLI_Command : public DisplayObject
{
	public:
		CLI_Command(State* s);
		~CLI_Command();

		void render();

		bool on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

	private:

		size_t cursor_index = 0;

		Text* command;
		Text* completion;

		void execute();
		void backspace();
		void update_completion();
};
