

#include <iostream>
#include <cmath> //log2(), exp2()

#include <SDL.h>

#include "collector.h"
#include "config.h"
#include "file.h"
#include "utils.h"
#include "grid.h"


//just to make the code cleaner
#define FILE_SIZE config->file_size
#define FILE_PAD config->file_padding
#define FILE_OFFSET (FILE_SIZE + FILE_PAD)
#define WINDOW_W config->window_w
#define SQUARE(x) (x*x)


Grid::Grid()
{

}

Grid::~Grid()
{

}

void Grid::render(file_list::iterator begin, file_list::iterator end)
{
	//draw the files
	setRenderDrawColor(renderer, config->get_color(FILL));

	// int old_x = (*begin)->rect.x;
	// int old_y = (*begin)->rect.y;
	// begin++;

	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;
		SDL_Rect rect = file->rect;
		rect.x += x_offset; //adjust position for centering
		rect.y -= y_offset; //adjust position for scroll

		// SDL_RenderDrawLine(renderer, old_x, old_y, file->rect.x, file->rect.y);
		// old_x = file->rect.x;
		// old_y = file->rect.y;

		if(rectInWindow(rect))
			SDL_RenderFillRect(renderer, &rect);
	}
}

//generates a stack of hilbert curves for this fileset
//updates every File->rect
void Grid::layout(file_list::iterator begin, file_list::iterator end)
{
	//find nearest power of 2 for the size of the H curve (flooring it)
	const int grid_size = exp2(floor(log2( (double)(WINDOW_W / FILE_OFFSET) )));

	//size of the grid in pixels (used for stacking multiple curves)
	const int grid_pixel_size = grid_size * FILE_OFFSET;

	//number of files in one hilbert curve
	const int d_per_hilbert = SQUARE(grid_size);

	//calculate the offset necessary to horizontally center the column
	x_offset = (WINDOW_W - (FILE_OFFSET * grid_size)) / 2;

	int i = 0; //which hilbert curve 
	int d = 0; //distance along the current hilbert curve

	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;

		//compute the Hilbert position
		int x = 0, y = 0;
		hilbert_d_to_point(grid_size, d, &x, &y);

		file->rect = {
			(x * FILE_OFFSET) + FILE_PAD,
			(y * FILE_OFFSET) + FILE_PAD + (i * grid_pixel_size),
			FILE_SIZE,
			FILE_SIZE
		};

		//handle looping over multiple hilbert curves
		d = (d + 1) % d_per_hilbert;
		if(d == 0) i++;
	}

	std::cout << i << std::endl;
}

void Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	y_offset -= (e.y * config->scroll_speed);

	if(y_offset < 0) y_offset = 0;
	else if(y_offset > max_scroll) y_offset = max_scroll;
}
