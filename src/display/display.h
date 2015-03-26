
#pragma once

#include <SDL.h>

#include <display/displayobject.h>
#include <events/selection.h>
#include <display/cli.h>
#include <display/grid.h>
#include <filestore/file.h>


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

		void on_selection(Selection* new_selection);

	private:
		DisplayObject* cli;
		DisplayObject* grid;

		Selection* selection; //the latest Selection from the FileStore

		void send_quit();
		void send_selector();
};
