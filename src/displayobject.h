
#pragma once

#include <SDL.h>

#include "selector.h"
#include "selection.h"
#include "file.h"

class DisplayObject
{
	public:
		virtual ~DisplayObject() {}

		//subclass may choose which to implement
		virtual void render() {}
		virtual void render(file_list_it begin, file_list_it end) {}
		virtual void render(file_list_it begin, file_list_it end, Selection* selection) {}

		//subclass may choose which to implement
		virtual void layout() {}
		virtual void layout(file_list_it begin, file_list_it end) {}
		virtual void layout(file_list_it begin, file_list_it end, Selection* selection) {}

		virtual void on_key(SDL_KeyboardEvent &e) {}
		virtual void on_text(SDL_TextInputEvent &e) {}
		virtual void on_wheel(SDL_MouseWheelEvent &e) {}
		virtual void fill_selector(Selector* selector) {}
		virtual void read_selection(Selection* selection) {}
};
