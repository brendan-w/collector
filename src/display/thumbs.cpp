

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
	const size_t height_files = (viewport.h > 0) ? (viewport.h / FILE_THUMB_OFFSET) : 1;
	const size_t height_px = height_files * FILE_THUMB_OFFSET;
	const size_t width_px = (s->size() / height_files) * FILE_THUMB_OFFSET;

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

	//in case the layout changed out from under the mouse
	update_hover();

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
	update_hover();
}

void Thumbs::on_click(SDL_MouseButtonEvent &e)
{
	if((e.button == SDL_BUTTON_LEFT) || (e.button == SDL_BUTTON_RIGHT))
	{
		bool include = (e.button == SDL_BUTTON_LEFT);
		state->toggle_inexclude(state->file_under_mouse, include);
		sdl->submit(STATE_CHANGE);
	}

	mark_dirty();
}

void Thumbs::on_motion(SDL_MouseMotionEvent &e)
{
	mouse = { e.x, e.y };
	update_hover();
}

void Thumbs::update_hover()
{
	File* old_file = state->file_under_mouse;
	state->file_under_mouse = mouse_to_file(mouse.x, mouse.y);

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
