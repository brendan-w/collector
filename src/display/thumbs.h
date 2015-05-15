
#pragma once

#include <SDL.h>

#include <display/displayobject.h>
#include <display/state.h>
#include <filestore/file.h>
#include <filestore/selection.h>


class Thumbs : public virtual DisplayObject
{
	public:
		Thumbs(State* s);
		~Thumbs();
		void render();
		void resize();

		void on_selection();
		void on_wheel(SDL_MouseWheelEvent &e);
		void on_click(SDL_MouseButtonEvent &e);
		void on_motion(SDL_MouseMotionEvent &e);

	private:

		SDL_Point mouse;

		size_t current_height_files = 0;

		void render_file(File* file);
		void update_hover();
		File* mouse_to_file(int x, int y);
};
