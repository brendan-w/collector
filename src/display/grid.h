
#pragma once

#include <SDL.h>

#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selection.h>


class Grid : public virtual DisplayObject
{
	public:
		Grid(Selection** s);
		~Grid();
		void render();
		void resize();

		bool on_motion(SDL_MouseMotionEvent &e);

	private:
		File* file_under_mouse = NULL;

		//used to prevent excessive recalculation during window resize
		size_t current_height_files = 0;

		void render_file(File* file, bool selected);
		File* mouse_to_file(int x, int y);
};
