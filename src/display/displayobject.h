
#pragma once

#include <SDL.h>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>


/*
	Note, this object is built for centered, vertical, scrollable columns of content
*/

class DisplayObject
{
	public:

		//all display objects must be initted with a pointer to the current state
		DisplayObject(Selection** _s);
		virtual ~DisplayObject() {}

		virtual void render() {}
		virtual void resize() {}

		//events, return bool for whether a state change merits broadcasting a new Selector
		virtual bool on_key(SDL_KeyboardEvent &e);
		virtual bool on_text(SDL_TextInputEvent &e);
		virtual bool on_wheel(SDL_MouseWheelEvent &e);
		virtual bool on_motion(SDL_MouseMotionEvent &e);

		//scroll utilities
		void pageup();
		void pagedown();
		void limit_scroll();

		//IO with the outside world
		virtual void on_selection() {}
		virtual void on_file_info(File* f) {}
		virtual void fill_selector(Selector* selector) {}

		bool is_dirty();

	protected:
		void mark_dirty() { dirty = true; }
		void set_scroll_range(size_t s);
		void set_centered_width(int w);
		int x_offset() { return offset.x; }
		int y_offset() { return offset.y; }
		Selection* selection() { return *s; }


	private:

		//pointer to the current selection pointer
		Selection** s;

		//does this widget need rendering
		bool dirty = true;

		// offset.x --> used for horizontal scrolling
		// offset.y --> used for vertically centering the row of files
		SDL_Point offset = {0, 0};
		size_t scroll_range = 0;
};
