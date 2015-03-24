
#pragma once

#include "file.h"


class Grid
{
	public:
		Grid();
		~Grid();
		void render(file_list::iterator begin, file_list::iterator end);
		void layout(file_list::iterator begin, file_list::iterator end);

		void on_wheel(SDL_MouseWheelEvent &e);
	private:

		int scroll = 0;
		int max_scroll = 200;
};

