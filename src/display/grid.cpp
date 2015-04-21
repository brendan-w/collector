

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
		file->point.x + x_offset(), //adjust position for centering
		file->point.y + y_offset(), //adjust position for scroll
		FILE_SIZE,
		FILE_SIZE
	};

	if(rectInWindow(rect))
	{
		bool under_mouse = (file == file_under_mouse);

		if(!selected && !under_mouse)
			setRenderDrawColor(config->get_color(FILE_NEUTRAL));

		else if(selected && !under_mouse)
			setRenderDrawColor(config->get_color(FILE_SELECTED));

		else if(!selected && under_mouse)
			setRenderDrawColor(config->get_color(FILE_NEUTRAL_HOVER));

		else if(selected && under_mouse)
			setRenderDrawColor(config->get_color(FILE_SELECTED_HOVER));

		SDL_RenderFillRect(renderer, &rect);
	}
}


//generates a stack of hilbert curves for this fileset
//updates every File->point
void Grid::layout(bool force)
{
	rect = config->get_window_rect();

	//find nearest power of 2 for the size of the H curve (flooring it)
	const int new_grid_size = exp2(floor(log2( (double)(WINDOW_W / FILE_OFFSET) )));

	//calculate the offset necessary to horizontally center the column
	set_centered_width(FILE_OFFSET * new_grid_size);

	//prevent excessive recomputation of the layout
	if(force || (grid_size != new_grid_size))
	{
		grid_size = new_grid_size;

		//size of the grid in pixels (used for stacking multiple curves)
		grid_pixel_size = grid_size * FILE_OFFSET;

		//number of files in one hilbert curve
		d_per_hilbert = SQUARE(grid_size);

		int d = 0; //distance along the current hilbert curve
		int scroll = 0;

		Selection* selection = get_selection();
		file_vector_it begin = selection->all_begin();
		file_vector_it end   = selection->all_end();

		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;

			//compute the Hilbert position
			SDL_Point h = hilbert_d_to_point(grid_size, d);

			file->point = {
				h.x * FILE_OFFSET,
				h.y * FILE_OFFSET
			};

			//update the maximum scroll limit
			if(scroll < file->point.y)
				scroll = file->point.y;

			d++;
		}

		scroll += FILE_OFFSET; //don't forget the last line of files have thickness
		
		//since the window was resized, check the scroll position
		set_scroll_height(scroll);
	}
}


bool Grid::on_motion(SDL_MouseMotionEvent &e)
{
	file_under_mouse = mouse_to_file(e.x, e.y);
	submit(FILE_INFO, (void*) file_under_mouse);
	return false;
}

//lookup the file under the cursor
File* Grid::mouse_to_file(int x, int y)
{
	//adjust for view offsets (scrolling & centering)
	SDL_Point m = {
		x - x_offset(),
		y - y_offset(),
	};

	//prevents negative numbers from reaching the division
	//save us from having to do a floor()
	if(m.x < 0)
		return NULL;

	//translate into H curve coordinate space
	m = {
		m.x / FILE_OFFSET,
		m.y / FILE_OFFSET
	};

	//check that the point is within the grid
	if((m.x < grid_size) && (m.y >= 0))
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
