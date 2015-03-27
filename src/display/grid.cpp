

#include <iostream>
#include <cmath> //log2(), exp2(), floor()

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <event.h>
#include <utils.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/grid.h>



Grid::Grid(Selection** s) : DisplayObject(s)
{

}

Grid::~Grid()
{

}

void Grid::render()
{
	Selection* selection = get_selection();
	file_vector_it begin = selection->all_begin();
	file_vector_it end   = selection->all_end();

	//not very dry, but saves having to check for a null selection every iteration
	if(selection->size() > 0)
	{
		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;
			render_file(file, selection->has(file));
		}
	}
	else
	{
		for(auto it = begin; it != end; ++it)
		{
			render_file(*it, false);
		}
	}
}


void Grid::render_file(File* file, bool selected)
{
	SDL_Rect rect = {
		file->point.x + offset.x, //adjust position for centering
		file->point.y - offset.y, //adjust position for scroll
		FILE_SIZE,
		FILE_SIZE
	};

	if(rectInWindow(rect))
	{
		if(selected)
			setRenderDrawColor(renderer, config->get_color(HIGHLIGHT));
		else
			setRenderDrawColor(renderer, config->get_color(FILL));

		SDL_RenderFillRect(renderer, &rect);
	}
}


//generates a stack of hilbert curves for this fileset
//updates every File->point
void Grid::layout()
{
	rect = config->get_window_rect();

	//find nearest power of 2 for the size of the H curve (flooring it)
	const int new_grid_size = exp2(floor(log2( (double)(WINDOW_W / FILE_OFFSET) )));

	//calculate the offset necessary to horizontally center the column
	offset.x = (WINDOW_W - (FILE_OFFSET * new_grid_size)) / 2;

	//prevent excessive recomputation of the layout
	if(grid_size != new_grid_size)
	{
		grid_size = new_grid_size;

		//size of the grid in pixels (used for stacking multiple curves)
		grid_pixel_size = grid_size * FILE_OFFSET;

		//number of files in one hilbert curve
		d_per_hilbert = SQUARE(grid_size);

		int d = 0; //distance along the current hilbert curve

		scroll_height = 0;

		Selection* selection = get_selection();
		file_vector_it begin = selection->all_begin();
		file_vector_it end   = selection->all_end();

		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;

			//compute the Hilbert position
			SDL_Point h = hilbert_d_to_point(grid_size, d);

			file->point = {
				(h.x * FILE_OFFSET) + FILE_PAD,
				(h.y * FILE_OFFSET) + FILE_PAD
			};

			//update the maximum scroll limit
			if(scroll_height < file->point.y)
				scroll_height = file->point.y;

			d++;
		}

		scroll_height += FILE_OFFSET; //don't forget the last line of files have thickness
		scroll_height += config->CLI_height;
	}

	//since the window was resized, check the scroll position
	limit_scroll();
}



bool Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	offset.y -= (e.y * config->scroll_speed);
	limit_scroll();
	return false;
}

bool Grid::on_motion(SDL_MouseMotionEvent &e)
{
	File* file = mouse_to_file(e.x, e.y);
	submit(FILE_INFO, (void*) file);
	return false;
}

void Grid::on_selection()
{

}

//lookup the file under the cursor
File* Grid::mouse_to_file(int x, int y)
{
	//adjust for view offsets (scrolling & centering)
	SDL_Point m = {
		x - offset.x,
		y + offset.y,
	};

	//translate into H curve coordinate space
	m = {
		(m.x - FILE_PAD) / FILE_OFFSET,
		(m.y - FILE_PAD) / FILE_OFFSET,		
	};

	//check that the point is within the grid
	if((m.x >= 0) && (m.x < grid_size) && (m.y >= 0))
	{
		size_t d = hilbert_point_to_d(grid_size, m);
		
		//check that the point isn't in a void
		//beyond the end the file list
		if(d < get_selection()->all_size())
		{
			//get this file by index
			return get_selection()->all_at(d);
		}
	}

	return NULL;
}

void Grid::limit_scroll()
{
	int max_scroll = scroll_height - WINDOW_H;

	if(max_scroll < 0) max_scroll = 0;

	if(offset.y < 0) offset.y = 0;
	else if(offset.y > max_scroll) offset.y = max_scroll;
}
