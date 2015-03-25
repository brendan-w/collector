
#pragma once

#include "selector.h"
#include "file.h"

class DisplayObject
{
	public:
		virtual ~DisplayObject() {}
		virtual void render() {}
		virtual void render(file_list::iterator begin, file_list::iterator end) {}
		virtual void layout(file_list::iterator begin, file_list::iterator end) {}
		virtual void on_key(SDL_KeyboardEvent &e) {}
		virtual void on_text(SDL_TextInputEvent &e) {}
		virtual void on_wheel(SDL_MouseWheelEvent &e) {}
		virtual void fill_selector(Selector* selector) {}
};
