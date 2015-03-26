
#pragma once

#include <SDL.h>

#include <events/selector.h>
#include <events/selection.h>
#include <filestore/file.h>

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

		//events, return bool for whether a state change merits broadcasting a new Selector
		virtual bool on_key(SDL_KeyboardEvent &e) { return false; }
		virtual bool on_text(SDL_TextInputEvent &e) { return false; }
		virtual bool on_wheel(SDL_MouseWheelEvent &e) { return false; }

		//IO with the outside world
		virtual void fill_selector(Selector* selector) {}
		virtual void read_selection(Selection* selection) {}

	protected:
		SDL_Rect rect;
};
