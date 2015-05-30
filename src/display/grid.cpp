
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# grid.cpp                                                             #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/grid.h>


#define GRID_V_PADDING (CLI_H * 2)
#define MINIMAP_H 8
#define MINIMAP_VIEWPORT_DIST 3



Grid::Grid(State* s) : DisplayObject(s)
{

}

Grid::~Grid()
{
	minimap.clear();
	clear_tags();
}

void Grid::render()
{
	SDL_Rect rect = sdl->get_viewport();
	sdl->set_color(BACKGROUND);
	sdl->fill_rect(rect);

	render_minimap();

	render_tags();

	Selection* s = selection();
	FileVector_it begin = s->all_begin();
	FileVector_it end   = s->all_end();

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

void Grid::render_minimap()
{
	SDL_Rect rect = sdl->get_viewport();

	rect.h = MINIMAP_H;
	rect.y = (GRID_V_PADDING - rect.h) / 2;

	sdl->set_color(FILE_NEUTRAL);
	sdl->fill_rect(rect);

	const double width = (double) rect.w;

	sdl->set_color(FILE_SELECTED);

	for(Bound b: minimap)
	{
		int lower = (int) (b.lower * width);
		int upper = (int) (b.upper * width);

		if(lower == upper) upper++;

		rect.x = lower;
		rect.w = upper - lower;
		sdl->fill_rect(rect);
	}

	//draw the current viewport region indicators

	const int x = (int) ((double) x_offset() * width / get_scroll_range());
	const int w = (int) ((double) width * width / get_scroll_range());

	const int top = rect.y - MINIMAP_VIEWPORT_DIST - 1; // -1 since the stroke is 1 pixel wide
	const int bottom = rect.y + rect.h + MINIMAP_VIEWPORT_DIST;

	sdl->draw_line(x, top, x + w, top);
	sdl->draw_line(x, bottom, x + w, bottom);
}

void Grid::render_tags()
{
	SDL_Rect rect = sdl->get_viewport();

	const int y = rect.h - GRID_V_PADDING + (GRID_V_PADDING / 2) - (CLI_H / 2);

	for(Grid_Tag t: tags)
	{
		int x = (t.x * FILE_OFFSET) - x_offset();
		t.text->render(x, y);
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
	clear_tags();
	Selection* s = selection();

	SDL_Rect viewport = sdl->get_viewport();
	const size_t total_files = s->all_size();
	const size_t height_files = (viewport.h > 0) ? (((viewport.h - GRID_V_PADDING * 2)) / FILE_OFFSET) : 1;
	const size_t height_px = height_files * FILE_OFFSET;
	size_t overflow_column = (total_files % height_files == 0) ? 0 : 1;
	const size_t width_files = (total_files / height_files) + overflow_column;
	const size_t width_px = width_files * FILE_OFFSET;

	set_scroll_range(width_px);
	set_centered_height(height_px);

	//don't recalc the tile positions unless we have to
	if(current_height_files != height_files)
	{
		current_height_files = height_files;
		current_width_files = width_files;

		FileVector_it begin = s->all_begin();
		FileVector_it end   = s->all_end();

		size_t count = 0;
		size_t column_wait = 0;
		for(auto it = begin; it != end; ++it)
		{
			File* file = *it;

			//compute the XY coordinates based on position in sequence
			file->grid_pos.x = count / height_files;
			file->grid_pos.y = count % height_files;

			//get the representative tag name
			if(column_wait == 0)
				column_wait = create_tag(file, file->grid_pos.x);
			else
				column_wait--;

			count++;
		}
	}

	//even if the grid_pos properties didn't change
	//we still need to update the centering values
	mark_dirty();
}

void Grid::update_minimap()
{
	//calculate the minimap
	Selection* s = selection();

	minimap.clear();

	FileVector_it begin = s->all_begin();
	FileVector_it end   = s->all_end();

	const size_t total = s->all_size();
	const size_t max_region_sep = current_height_files;

	size_t count = 0;
	size_t files_since_last_selected = 0;
	bool in_region = false;

	Bound region;

	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;

		if(s->has(file))
		{
			files_since_last_selected = 0;

			if(!in_region)
			{
				in_region = true;
				region.lower = (double) count / (double) total;
			}
		}
		else
		{
			files_since_last_selected++;
			
			if(in_region)
			{
				if(files_since_last_selected >= max_region_sep)
				{
					in_region = false;
					size_t i = (count - files_since_last_selected);
					region.upper = (double) i / (double) total;
					minimap.push_back(region);
				}
			}
		}

		count++;
	}

	//close the last region, if there is one
	if(in_region)
	{
		in_region = false;
		region.upper = (count - files_since_last_selected);
		minimap.push_back(region);
	}
}

void Grid::auto_scroll()
{
	//read the minimap to determine where to scroll to

	if(state->should_autoscroll)
	{
		state->should_autoscroll = false;

		if(selection()->size() > 0)
		{
			const double single_file = (1.0 / selection()->all_size());

			Bound bound;
			double largest;

			//find the widest region of files
			for(Bound b: minimap)
			{
				if(b.upper == b.lower)
					b.upper += single_file;

				double size = b.upper - b.lower;
				if(size > largest)
				{
					bound = b;
					largest = size;
				}
			}

			//get the midpoint of the region
			double center = (bound.lower + bound.upper) / 2.0;
			int pos = ((int) (center * get_scroll_range()));

			//scroll to the region
			scroll_to(pos);
		}
	}
}

void Grid::on_selection()
{
	update_minimap();
	auto_scroll();
	mark_dirty();
}

void Grid::on_wheel(SDL_MouseWheelEvent &e)
{
	DisplayObject::on_wheel(e);
	mark_dirty();
}

void Grid::on_click(SDL_MouseButtonEvent &e)
{
	if(state->key_ctrl)
	{
		///CTRL clicking will include/exclude files
		if((e.button == SDL_BUTTON_LEFT) || (e.button == SDL_BUTTON_RIGHT))
		{
			bool include = (e.button == SDL_BUTTON_LEFT);
			state->toggle_inexclude(state->file_under_mouse, include);
			sdl->submit(STATE_CHANGE);
		}

		mark_dirty();
	}
	else if(e.clicks == 2)
	{
		//double clicking will open the file
		File* f = state->file_under_mouse;
		if(f != NULL)
			f->open(f->get_full_path());
	}
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

size_t Grid::create_tag(File* file, size_t column)
{
	Tag tag = file->get_exemplar_tag();

	if(tag.length() > 0)
	{
		if((tags.size() == 0) || (tag != tags.back().text->get_text()))
		{
			Grid_Tag t = {
				column,
				new Text(tag, config->get_color(CLI_DARK))
			};

			tags.push_back(t);

			//TODO: make this not hacky
			//number of columns to wait until the next tag
			return current_height_files * t.text->get_text().length() + 5; 
		}
	}

	return 0;
}

void Grid::clear_tags()
{
	for(Grid_Tag t: tags)
		delete t.text;
	tags.clear();
}
