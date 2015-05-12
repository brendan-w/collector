
#pragma once

#include <SDL.h>

#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selection.h>


class Thumbs : public virtual DisplayObject
{
	public:
		Thumbs(Selection** s);
		~Thumbs();
		void render();
		void resize();

		void on_selection();
		void on_wheel(SDL_MouseWheelEvent &e);
		void on_motion(SDL_MouseMotionEvent &e);

	private:

		SDL_Point mouse;
		File* file_under_mouse = NULL;

		size_t current_height_files = 0;

		void render_file(File* file);
		void update_hover();
		File* mouse_to_file(int x, int y);
};
