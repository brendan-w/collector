
#pragma once

#include "displayobject.h"
#include "file.h"


class Grid : public virtual DisplayObject
{
	public:
		Grid();
		~Grid();
		void render(file_list_it begin, file_list_it end);
		void render(file_list_it begin, file_list_it end, Selection* selection);
		
		void layout(file_list_it begin, file_list_it end);

		void on_wheel(SDL_MouseWheelEvent &e);
		void read_selection(Selection* selection);

	private:
		int current_grid_size = 0;
		int x_offset = 0; //used for centering the column of files
		int y_offset = 0; //used for vertical scrolling
		int scroll_height = 0; //the height of the entire scrollable region

		void render_file(File* file);
		void limit_scroll();
};

