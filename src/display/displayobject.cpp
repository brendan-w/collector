
#include <iostream>

#include <SDL.h>

#include <collector.h>
#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>




DisplayObject::DisplayObject(State* s) : state(s)
{

}


void DisplayObject::on_wheel(SDL_MouseWheelEvent &e)
{
	int old_offset = offset.x;

	offset.x -= (e.y * config->scroll_speed);
	limit_scroll();

	if(offset.x != old_offset)
		mark_dirty();
}


/*
	Optional tools for scrolling and centering
*/

void DisplayObject::pageup()
{
	int old_offset = offset.x;
	offset.x -= (sdl->get_viewport().w);
	limit_scroll();

	if(offset.x != old_offset)
		mark_dirty();
}

void DisplayObject::pagedown()
{
	int old_offset = offset.x;
	offset.x += (sdl->get_viewport().w);
	limit_scroll();

	if(offset.x != old_offset)
		mark_dirty();
}

bool DisplayObject::is_dirty()
{
	bool current = dirty;
	dirty = false;
	return current;
}

void DisplayObject::set_centered_height(size_t h)
{
	offset.y = ((sdl->get_viewport().h) - h) / 2;
}

void DisplayObject::set_scroll_range(size_t s)
{
	//account for the two UI bars at the top and bottom
	scroll_range = s;
	limit_scroll();
}

void DisplayObject::scroll_to(int s)
{
	s -= (sdl->get_viewport().w / 2);
	offset.x = s;
	limit_scroll();
}

void DisplayObject::limit_scroll()
{
	int max_scroll = scroll_range - (sdl->get_viewport().w);

	if(max_scroll < 0) max_scroll = 0;

	if(offset.x < 0) offset.x = 0;
	else if(offset.x > max_scroll) offset.x = max_scroll;
}
