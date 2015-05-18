
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# thumbs.cpp                                                           #
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
#include <thumbnail.h>
#include <event.h>
#include <utils.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/thumbs.h>



Thumbs::Thumbs(State* s) : DisplayObject(s)
{

}

Thumbs::~Thumbs()
{

}

void Thumbs::render()
{
	SDL_Rect rect = sdl->get_viewport();
	sdl->set_color(BACKGROUND);
	sdl->fill_rect(rect);

	//divider line
	sdl->set_color(OVERLAY);
	sdl->draw_line(rect.x, 0, rect.w, 0);

	for(File* file : *selection())
	{
		render_file(file);
	}
}


void Thumbs::render_file(File* file)
{
	SDL_Rect rect = {
		(file->thumb_pos.x * FILE_THUMB_OFFSET) - x_offset(),
		(file->thumb_pos.y * FILE_THUMB_OFFSET) + y_offset(),
		FILE_THUMB_SIZE,
		FILE_THUMB_SIZE
	};

	if(sdl->rect_in_viewport(rect))
	{
		sdl->set_color(FILE_NEUTRAL);
		file->get_thumb()->render(&rect);

		//if this file was manually included,
		//draw the green border marking it as included
		if(state->inexclude_has(file))
		{
			if(state->inexclude[file])
			{
				sdl->set_color(THUMB_INCLUDED);
				sdl->fill_rect(rect);
			}
		}
	}
}


//updates every File->thumb_pos
void Thumbs::resize()
{
	Selection* s = selection();

	SDL_Rect viewport = sdl->get_viewport();
	const size_t total_files = s->size();
	const size_t height_files = (viewport.h > 0) ? (viewport.h / FILE_THUMB_OFFSET) : 1;
	const size_t height_px = height_files * FILE_THUMB_OFFSET;
	size_t overflow_column = (total_files % height_files == 0) ? 0 : 1;
	const size_t width_files = (total_files / height_files) + overflow_column;
	const size_t width_px = width_files * FILE_THUMB_OFFSET;

	set_scroll_range(width_px);
	set_centered_height(height_px);

	file_set_it begin = s->begin();
	file_set_it end   = s->end();

	size_t count = 0;
	for(auto it = begin; it != end; ++it)
	{
		File* file = *it;
		//compute the XY coordinates based on position in sequence
		file->thumb_pos.x = count / height_files;
		file->thumb_pos.y = count % height_files;
		count++;
	}

	current_height_files = height_files;

	//even if the thumb_pos properties didn't change
	//we still need to update the centering values
	mark_dirty();
}

void Thumbs::on_selection()
{
	mark_dirty();
}

void Thumbs::on_wheel(SDL_MouseWheelEvent &e)
{
	DisplayObject::on_wheel(e);
	mark_dirty();
}

void Thumbs::on_click(SDL_MouseButtonEvent &e)
{
	if(state->key_ctrl)
	{
		//CTRL clicking will include/exclude files
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
		{
			selection()->export_();
			f->open(f->get_link_path());
		}
	}



}

void Thumbs::on_motion(SDL_MouseMotionEvent &e)
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
File* Thumbs::mouse_to_file(int x, int y)
{
	SDL_Point m = {
		x + x_offset(),
		y - y_offset()
	};

	if((m.x < 0) || (m.y < 0))
		return NULL;

	if(m.y >= (int)(current_height_files * FILE_THUMB_OFFSET))
		return NULL;

	m.x = m.x / FILE_THUMB_OFFSET;
	m.y = m.y / FILE_THUMB_OFFSET;

	size_t i = (m.x * current_height_files) + m.y;

	if(i < selection()->size())
	{
		//get this file by index
		return selection()->at(i);
	}

	return NULL;
}
