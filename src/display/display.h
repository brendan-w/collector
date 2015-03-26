
#pragma once

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>
#include <display/grid.h>
#include <display/cli.h>


class Display
{
	public:
		Display(Selection* init_selection);
		~Display();

		void render();

		void on_resize();
		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);
		void on_wheel(SDL_MouseWheelEvent &e);

		void on_selection(Selection* new_selection);

	private:
		DisplayObject* cli;
		DisplayObject* grid;

		Selection* selection; //the latest Selection from the FileStore

		void send_quit();
		void send_selector();
};
