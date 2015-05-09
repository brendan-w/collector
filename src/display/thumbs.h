
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

		void on_motion(SDL_MouseMotionEvent &e);
		void on_selection();

	private:

		File* file_under_mouse = NULL;

		size_t current_height_files = 0;

		void render_file(File* file);
		File* mouse_to_file(int x, int y);
};
