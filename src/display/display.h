
#pragma once

#include <vector>

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>
#include <display/cli.h>
#include <display/info.h>
#include <display/grid.h>


#define view views[current_view]


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

		//fixed objects, always present
		DisplayObject* cli;
		DisplayObject* info;

		//the current view
		size_t current_view;

		//array of all possible main displays
		std::vector<DisplayObject*> views;

		void cycle_view();
		void send_quit();
		void send_selector();
};
