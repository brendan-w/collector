
#pragma once

#include "SDL.h"

#include <texture.h>
#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>


class Grid : public virtual DisplayObject
{
	public:
		Grid();
		~Grid();
		void render(Selection* selection);
		void layout(Selection* selection);

		bool on_wheel(SDL_MouseWheelEvent &e);

		void read_selection(Selection* selection);

	private:
		SDL_Surface* surface;

		int current_grid_size = 0;
		int x_offset = 0; //used for centering the column of files
		int y_offset = 0; //used for vertical scrolling
		int scroll_height = 0; //the height of the entire scrollable region

		void render_file(File* file, bool selected);
		void limit_scroll();
};
