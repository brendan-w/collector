
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
		int x_offset = 0; //used for centering the column of files
		int y_offset = 0; //used for vertical scrolling
		int max_scroll = 200;

		void render_file(File* file);
};

