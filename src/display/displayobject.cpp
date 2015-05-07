

#include <SDL.h>

#include <collector.h>
#include <filestore/selection.h>
#include <display/displayobject.h>



DisplayObject::DisplayObject(Selection** _s)
{
	s = _s;
}


void DisplayObject::on_wheel(SDL_MouseWheelEvent &e)
{
	offset.x -= (e.y * config->scroll_speed);
	limit_scroll();
}


/*
	Optional tools for scrolling and centering
*/

void DisplayObject::pageup()
{
	offset.x -= (sdl->window_size().x);
	limit_scroll();
}

void DisplayObject::pagedown()
{
	offset.x += (sdl->window_size().x);
	limit_scroll();
}

bool DisplayObject::is_dirty()
{
	bool state = dirty;
	dirty = false;
	return state;
}

void DisplayObject::set_centered_height(size_t h)
{
	offset.y = ((sdl->window_size().y) - h) / 2;
}

void DisplayObject::set_scroll_range(size_t s)
{
	//account for the two UI bars at the top and bottom
	scroll_range = s + (2 * CLI_H);
	limit_scroll();
}

void DisplayObject::limit_scroll()
{
	int max_scroll = scroll_range - (sdl->window_size().x);

	if(max_scroll < 0) max_scroll = 0;

	if(offset.x < 0) offset.x = 0;
	else if(offset.x > max_scroll) offset.x = max_scroll;
}
