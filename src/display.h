
#pragma once

#include <SDL.h>

#include "displayobject.h"
#include "cli.h"
#include "grid.h"
#include "file.h"


class Display
{
	public:
		Display();
		~Display();
		void render(file_list::iterator begin, file_list::iterator end);
		void on_resize(file_list::iterator begin, file_list::iterator end);
		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);
		void on_wheel(SDL_MouseWheelEvent &e);

	private:
		DisplayObject* cli;
		DisplayObject* grid;

		void send_quit();
		void send_selector();
};
