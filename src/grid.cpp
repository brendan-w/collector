

#include <iostream>
#include <cmath> //log2()

#include <SDL.h>

#include "collector.h"
#include "config.h"
#include "file.h"
#include "utils.h"
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

	//find the number of files wide the display is
	int width = config->window_w / (config->file_size + config->file_padding);

	//find nearest power of 2 for the size of the H curve
	int grid_size = exp2(ceil(log2( (double) width)));

	const int multiplier = config->file_size + config->file_padding;

	int d = 0;
	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;

		//compute the Hilbert position
		int x = 0, y = 0;
		d2xy(grid_size, d, &x, &y);

		file->rect = {
			(x * multiplier) + config->file_padding,
			(y * multiplier) + config->file_padding,
			config->file_size,
			config->file_size
		};

		d++;
	}

	std::cout << grid_size << std::endl;
}
