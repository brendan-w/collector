
#pragma once

#include <vector>

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>



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

		DisplayObject* current; //the view currently under the mouse

		DisplayObject* cli;
		DisplayObject* info;
		DisplayObject* grid;
		DisplayObject* thumbs;

		void send_selector();
};
