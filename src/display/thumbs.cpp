

#include <iostream>

#include <SDL.h>

#include <collector.h>
#include <thumbnail.h>
#include <event.h>
#include <utils.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/thumbs.h>



Thumbs::Thumbs(Selection** s) : DisplayObject(s)
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

	sdl->set_color(FILE_NEUTRAL);
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
		file->get_thumb()->render(&rect);
}


//updates every File->thumb_pos
void Thumbs::resize()
{
	Selection* s = selection();

	SDL_Rect viewport = sdl->get_viewport();
	const size_t height_files = (viewport.h > 0) ? (((viewport.h - CLI_H * 2)) / FILE_THUMB_OFFSET) : 1;
	const size_t height_px = height_files * FILE_THUMB_OFFSET;
	const size_t width_px = (s->size() / height_files) * FILE_THUMB_OFFSET;

	set_scroll_range(width_px);
	set_centered_height(height_px);

	//don't recalc the tile positions unless we have to
	if(current_height_files != height_files)
	{
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
	}

	//even if the thumb_pos properties didn't change
	//we still need to update the centering values
	mark_dirty();
}

void Thumbs::on_selection()
{
	//the new selection's layout must be computed
	resize();
	//mark_dirty(); //called in resize()
}

void Thumbs::on_motion(SDL_MouseMotionEvent &e)
{
	File* old_file = file_under_mouse;
	file_under_mouse = mouse_to_file(e.x, e.y);

	if(file_under_mouse != old_file)
	{
		sdl->submit(FILE_INFO, (void*) file_under_mouse);
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
