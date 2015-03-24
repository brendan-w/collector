


#include <SDL.h>

#include "collector.h"
#include "config.h"
#include "file.h"
#include "grid.h"


Grid::Grid()
{

}

Grid::~Grid()
{
	
}

void Grid::render(file_list::iterator begin, file_list::iterator end)
{
	setRenderDrawColor(renderer, config->get_color(FILL));

	for( ; begin != end; ++begin)
	{
		File* file = *begin;
		SDL_RenderFillRect(renderer, &(file->rect));
	}
}

void Grid::layout(file_list::iterator begin, file_list::iterator end)
{
	int x = 1;
	int y = 1;
	int inc = 6;

	for( ; begin != end; ++begin)
	{
		File* file = *begin;
		file->rect = { x, y, 5, 5 };

		x += inc;
		if(x > config->window_w)
		{
			x = 1;
			y += inc;
		}
	}
}
