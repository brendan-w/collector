
#pragma once

#include "file.h"


class Grid
{
	public:
		Grid();
		~Grid();
		void render(file_list::iterator begin, file_list::iterator end);
		void layout(file_list::iterator begin, file_list::iterator end);
	private:
};

