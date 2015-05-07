

#include <iostream>

#include <SDL.h>

#include <collector.h>
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
	SDL_Rect rect = sdl->get_viewport();
	sdl->set_color(BACKGROUND);
	sdl->fill_rect(rect);

	Selection* s = selection();
	file_vector_it begin = s->all_begin();
	file_vector_it end   = s->all_end();

	//not very dry, but saves having to check for a null selection every iteration
	if(s->size() > 0)
	{
		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;
			render_file(file, s->has(file));
		}
	}
	else
	{
		for(auto it = begin; it != end; ++it)
		{
			render_file(*it, false);
		}
	}

	//divider line
	sdl->set_color(OVERLAY);
	sdl->draw_line(rect.x, rect.h - 1, rect.w, rect.h - 1);
}


void Grid::render_file(File* file, bool selected)
{
	SDL_Rect rect = {
		(file->grid_pos.x * FILE_OFFSET) + x_offset(),
		(file->grid_pos.y * FILE_OFFSET) + y_offset(),
		FILE_SIZE,
		FILE_SIZE
	};

	if(sdl->rect_in_window(rect))
	{
		bool under_mouse = (file == file_under_mouse);

		if(!selected && !under_mouse)
			sdl->set_color(FILE_NEUTRAL);

		else if(selected && !under_mouse)
			sdl->set_color(FILE_SELECTED);

		else if(!selected && under_mouse)
			sdl->set_color(FILE_NEUTRAL_HOVER);

		else if(selected && under_mouse)
			sdl->set_color(FILE_SELECTED_HOVER);

		sdl->fill_rect(rect);
	}
}


//updates every File->grid_pos
void Grid::resize()
{
	SDL_Rect viewport = sdl->get_viewport();
	const size_t height_files = (viewport.h > 0) ? (((viewport.h - CLI_H * 2)) / FILE_OFFSET) : 1;
	const size_t height_px = height_files * FILE_OFFSET;

	set_centered_height(height_px);

	//don't recalc the tile positions unless we have to
	if(current_height_files != height_files)
	{

		Selection* s = selection();
		file_vector_it begin = s->all_begin();
		file_vector_it end   = s->all_end();

		size_t count = 0;
		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;
			//compute the XY coordinates based on position in sequence
			file->grid_pos.x = count / height_files;
			file->grid_pos.y = count % height_files;
			count++;
		}

		current_height_files = height_files;
	}

	mark_dirty();
}


void Grid::on_motion(SDL_MouseMotionEvent &e)
{
	// file_under_mouse = mouse_to_file(e.x, e.y);
	// sdl->submit(FILE_INFO, (void*) file_under_mouse);
}

//lookup the file under the cursor
File* Grid::mouse_to_file(int x, int y)
{
	/*
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
		if(d < selection()->all_size())
		{
			//get this file by index
			return selection()->all_at(d);
		}
	}
	*/

	return NULL;
}
