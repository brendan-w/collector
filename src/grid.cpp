

#include <iostream>

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

	int old_x = (*begin)->rect.x;
	int old_y = (*begin)->rect.y;

	begin++;

	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;
		SDL_RenderDrawLine(renderer, old_x, old_y, file->rect.x, file->rect.y);
		old_x = file->rect.x;
		old_y = file->rect.y;

		// SDL_RenderFillRect(renderer, &(file->rect));
	}
}

//generate the hilbert curve for this fileset
void Grid::layout(file_list::iterator begin, file_list::iterator end)
{
	//raster layout
	
	int x = config->file_padding;
	int y = config->file_padding;
	int inc = config->file_size + config->file_padding;

	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;
		file->rect = {
			x,
			y,
			config->file_size,
			config->file_size };

		x += inc;
		if(x > config->window_w)
		{
			x = config->file_padding;
			y += inc;
		}
	}

	//size of the hilbert curve (number of squares (files) wide)
	int size = config->window_w / (config->file_size + config->file_padding);

	std::cout << size << std::endl;
}
