

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
#define WINDOW_W (config->window_w)
#define WINDOW_H (config->window_h)
#define SQUARE(x) (x*x)


Grid::Grid()
{

}

Grid::~Grid()
{

}

void Grid::render(file_list_it begin, file_list_it end, Selection* selection)
{
	//draw the files
	render(begin, end);

	//draw selected files
	setRenderDrawColor(renderer, config->get_color(HIGHLIGHT));
	file_set selected = selection->get_files();

	for(File* file: selected)
	{
		render_file(file);
	}
}

void Grid::render(file_list_it begin, file_list_it end)
{
	//draw all files
	setRenderDrawColor(renderer, config->get_color(FILL));

	for(auto it = begin; it != end; ++it)
	{
		render_file(*it);
	}
}


void Grid::render_file(File* file)
{
		SDL_Rect rect = {
			file->point.x + x_offset, //adjust position for centering
			file->point.y - y_offset, //adjust position for scroll
			FILE_SIZE,
			FILE_SIZE
		};

		if(rectInWindow(rect))
			SDL_RenderFillRect(renderer, &rect);
}

//generates a stack of hilbert curves for this fileset
//updates every File->point
void Grid::layout(file_list_it begin, file_list_it end)
{
	//find nearest power of 2 for the size of the H curve (flooring it)
	const int grid_size = exp2(floor(log2( (double)(WINDOW_W / FILE_OFFSET) )));

	//calculate the offset necessary to horizontally center the column
	x_offset = (WINDOW_W - (FILE_OFFSET * grid_size)) / 2;

	//prevent excessive recomputation of the layout
	if(current_grid_size != grid_size)
	{
		current_grid_size = grid_size;

		//size of the grid in pixels (used for stacking multiple curves)
		const int grid_pixel_size = grid_size * FILE_OFFSET;

		//number of files in one hilbert curve
		const int d_per_hilbert = SQUARE(grid_size);

		int i = 0; //which hilbert curve 
		int d = 0; //distance along the current hilbert curve

		scroll_height = 0;

		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;

			//compute the Hilbert position
			int x = 0, y = 0;
			hilbert_d_to_point(grid_size, d, &x, &y);

			file->point = {
				(x * FILE_OFFSET) + FILE_PAD,
				(y * FILE_OFFSET) + FILE_PAD + (i * grid_pixel_size)
			};

			//update the maximum scroll limit
			if(scroll_height < file->point.y)
				scroll_height = file->point.y;

			//handle looping over multiple hilbert curves
			d = (d + 1) % d_per_hilbert;
			if(d == 0) i++;
		}

		scroll_height += FILE_OFFSET; //don't forget the last line of files have thickness
		scroll_height += config->CLI_height;
	}

	limit_scroll();
}

void Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	y_offset -= (e.y * config->scroll_speed);
	limit_scroll();
}

void Grid::read_selection(Selection* selection)
{

}

void Grid::limit_scroll()
{
	int max_scroll = scroll_height - WINDOW_H;

	if(max_scroll < 0) max_scroll = 0;

	if(y_offset < 0) y_offset = 0;
	else if(y_offset > max_scroll) y_offset = max_scroll;
}