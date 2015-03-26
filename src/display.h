
#pragma once

#include <SDL.h>

#include "displayobject.h"
#include "selection.h"
#include "cli.h"
#include "grid.h"
#include "file.h"


class Display
{
	public:
		Display();
		~Display();
		void render(file_list_it begin, file_list_it end);
		void on_resize(file_list_it begin, file_list_it end);
		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);
		void on_wheel(SDL_MouseWheelEvent &e);

		void on_select(Selection* new_selection);

	private:
		DisplayObject* cli;
		DisplayObject* grid;

		Selection* selection; //the latest Selection from the FileStore

		void send_quit();
		void send_selector();
};
