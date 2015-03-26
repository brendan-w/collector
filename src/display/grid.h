
#pragma once

#include <SDL.h>

#include <texture.h>
#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>


class Grid : public virtual DisplayObject
{
	public:
		Grid(Selection** s);
		~Grid();
		void render();
		void layout();

		bool on_wheel(SDL_MouseWheelEvent &e);
		bool on_motion(SDL_MouseMotionEvent &e);

		void on_selection();

	private:
		SDL_Surface* surface;

		//metrics on the hilbert curves currently on screen
		int grid_size = 0;
		int grid_pixel_size = 0;
		int d_per_hilbert = 0;
		
		// offset.x --> used for centering the column of files
		// offset.y --> used for vertical scrolling
		SDL_Point offset;

		//the height of the entire scrollable region
		int scroll_height = 0;

		void render_file(File* file, bool selected);
		void limit_scroll();
};
