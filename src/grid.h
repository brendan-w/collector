
#pragma once

#include "displayobject.h"
#include "file.h"


class Grid : public virtual DisplayObject
{
	public:
		Grid();
		~Grid();
		void render(file_list::iterator begin, file_list::iterator end);
		void layout(file_list::iterator begin, file_list::iterator end);

		void on_wheel(SDL_MouseWheelEvent &e);

	private:
		int x_offset = 0; //used for centering the column of files
		int y_offset = 0; //used for vertical scrolling
		int max_scroll = 200;
};

