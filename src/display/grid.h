
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
		void layout(bool force);

		bool on_motion(SDL_MouseMotionEvent &e);

	private:
		File* file_under_mouse = NULL;

		//metrics on the hilbert curves currently on screen
		int grid_size = 0;
		int grid_pixel_size = 0;
		int d_per_hilbert = 0;

		void render_file(File* file, bool selected);
		File* mouse_to_file(int x, int y);
};
