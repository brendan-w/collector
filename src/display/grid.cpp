
#include <SDL.h>

#include <collector.h>
#include <utils.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>
#include <display/grid.h>



Grid::Grid(Selection** s) : DisplayObject(s)
{

}

Grid::~Grid()
{
	inexclude.clear();
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
		(file->grid_pos.x * FILE_OFFSET) - x_offset(),
		(file->grid_pos.y * FILE_OFFSET) + y_offset(),
		FILE_SIZE,
		FILE_SIZE
	};

	if(sdl->rect_in_viewport(rect))
	{
		bool under_mouse = (file == file_under_mouse);
		
		if(inexclude.find(file) != inexclude.end())
		{
			if(inexclude[file])
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

void Grid::on_motion(SDL_MouseMotionEvent &e)
{
	mouse = { e.x, e.y };
	update_hover();
}

void Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	DisplayObject::on_wheel(e);
	update_hover();
}

void Grid::on_click(SDL_MouseButtonEvent &e)
{
	File* f = file_under_mouse;
	if(f != NULL)
	{
		if(e.button == SDL_BUTTON_LEFT)
		{
			//include file
			if(inexclude.find(f) == inexclude.end())
				inexclude[f] = true;
			else
				inexclude.erase(f);
		}
		else if(e.button == SDL_BUTTON_RIGHT)
		{
			//exclude file
			if(inexclude.find(f) == inexclude.end())
				inexclude[f] = false;
			else
				inexclude.erase(f);
		}
	}
}

void Grid::update_hover()
{
	File* old_file = file_under_mouse;
	file_under_mouse = mouse_to_file(mouse.x, mouse.y);

	if(file_under_mouse != old_file)
	{
		sdl->submit(FILE_INFO, (void*) file_under_mouse);
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
