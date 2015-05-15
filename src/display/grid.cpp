
#include <iostream>

#include <SDL.h>

#include <collector.h>
#include <utils.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <display/grid.h>



Grid::Grid(State* s) : DisplayObject(s)
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
}


void Grid::render_file(File* file, bool selected)
{
	SDL_Rect rect = {
		(file->grid_pos.x * FILE_OFFSET) - x_offset(),
		(file->grid_pos.y * FILE_OFFSET) + y_offset(),
		FILE_SIZE,
		FILE_SIZE
	};

	if(sdl->rect_in_viewport(rect))
	{
		bool under_mouse = (file == state->file_under_mouse);
		
		if(state->inexclude_has(file))
		{
			if(state->inexclude[file])
			{
				//included
				if(under_mouse)
					sdl->set_color(FILE_INCLUDED_HOVER);
				else
					sdl->set_color(FILE_INCLUDED);
			}
			else
			{
				//excluded
				if(under_mouse)
					sdl->set_color(FILE_EXCLUDED_HOVER);
				else
					sdl->set_color(FILE_EXCLUDED);
			}
		}
		else if(!selected && !under_mouse)
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
	Selection* s = selection();

	SDL_Rect viewport = sdl->get_viewport();
	const size_t total_files = s->all_size();
	const size_t height_files = (viewport.h > 0) ? (((viewport.h - CLI_H * 2)) / FILE_OFFSET) : 1;
	const size_t height_px = height_files * FILE_OFFSET;
	const size_t width_files = (total_files / height_files);
	const size_t width_px = width_files * FILE_OFFSET;

	set_scroll_range(width_px);
	set_centered_height(height_px);

	//don't recalc the tile positions unless we have to
	if(current_height_files != height_files)
	{
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
		current_width_files = width_files;
	}

	//even if the grid_pos properties didn't change
	//we still need to update the centering values
	mark_dirty();
}

void Grid::on_selection()
{
	mark_dirty();
}

void Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	DisplayObject::on_wheel(e);
	mark_dirty();
}

void Grid::on_click(SDL_MouseButtonEvent &e)
{
	if((e.button == SDL_BUTTON_LEFT) || (e.button == SDL_BUTTON_RIGHT))
	{
		bool include = (e.button == SDL_BUTTON_LEFT);
		state->toggle_inexclude(state->file_under_mouse, include);
		sdl->submit(STATE_CHANGE);
	}

	mark_dirty();
}

void Grid::on_motion(SDL_MouseMotionEvent &e)
{
	File* old_file = state->file_under_mouse;
	state->file_under_mouse = mouse_to_file(e.x, e.y);

	if(state->file_under_mouse != old_file)
	{
		sdl->submit(STATE_CHANGE);
		mark_dirty();
	}
}

//lookup the file under the cursor
File* Grid::mouse_to_file(int x, int y)
{
	SDL_Point m = {
		x + x_offset(),
		y - y_offset()
	};

	if((m.x < 0) || (m.y < 0))
		return NULL;

	if(m.y >= (int)(current_height_files * FILE_OFFSET))
		return NULL;

	m.x = m.x / FILE_OFFSET;
	m.y = m.y / FILE_OFFSET;

	size_t i = (m.x * current_height_files) + m.y;

	if(i < selection()->all_size())
	{
		//get this file by index
		return selection()->all_at(i);
	}

	return NULL;
}
