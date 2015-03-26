
#pragma once

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>
#include <display/cli.h>
#include <display/info.h>
#include <display/grid.h>


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
		void on_motion(SDL_MouseMotionEvent &e);

		void on_selection(Selection* new_selection);
		void on_file_info(File* f);

	private:
		//the latest Selection from the FileStore
		//all children are given a pointer to this pointer
		Selection* selection;

		DisplayObject* cli;
		DisplayObject* info;
		DisplayObject* grid;

		void send_quit();
		void send_selector();
};
