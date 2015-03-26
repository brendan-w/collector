
#pragma once

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>

class DisplayObject
{
	public:

		//all display objects must be initted with a pointer to the current state
		DisplayObject(Selection** s) { selection = s; }
		virtual ~DisplayObject() {}

		virtual void render() {}
		virtual void layout() {}

		//events, return bool for whether a state change merits broadcasting a new Selector
		virtual bool on_key(SDL_KeyboardEvent &e) { return false; }
		virtual bool on_text(SDL_TextInputEvent &e) { return false; }
		virtual bool on_wheel(SDL_MouseWheelEvent &e) { return false; }
		virtual bool on_motion(SDL_MouseMotionEvent &e) { return false; }

		//IO with the outside world
		virtual void on_selection() {}
		virtual void on_file_info(File* f) {}
		virtual void fill_selector(Selector* selector) {}

	protected:
		Selection* current() { return *selection; }

		SDL_Rect rect;

	private:
		Selection** selection;
};
