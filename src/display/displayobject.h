
#pragma once

#include <SDL.h>

#include <filestore/types.h>


/*
	Note, this object is built for centered, vertical, scrollable columns of content
*/

class DisplayObject
{
	public:

		//all display objects must be initted with a pointer to the current state
		DisplayObject(Selection** _selection);
		virtual ~DisplayObject() {}

		virtual void render() {}
		virtual void resize() { mark_dirty(); }

		//events
		virtual void on_key(SDL_KeyboardEvent &e) {}
		virtual void on_text(SDL_TextInputEvent &e) {}
		virtual void on_wheel(SDL_MouseWheelEvent &e);
		virtual void on_click(SDL_MouseButtonEvent &e) {}
		virtual void on_motion(SDL_MouseMotionEvent &e) {}

		//scroll utilities
		void pageup();
		void pagedown();
		void limit_scroll();

		//IO with the outside world
		virtual void on_selection() {}
		virtual void fill_selector(Selector* selector) {}
		virtual void on_file_info(File* f) {}

		bool is_dirty();
		void mark_dirty() { dirty = true; }

	protected:
		void set_scroll_range(size_t s);
		void set_centered_height(size_t h);
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
