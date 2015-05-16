
#pragma once

#include <SDL.h>

#include <filestore/types.h>
#include <display/state.h>


/*
	Note, this object is built for centered, vertical, scrollable columns of content
*/

class DisplayObject
{
	public:

		//all display objects must be initted with a pointer to the current state
		DisplayObject(State* s);
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
		virtual void on_state_change() {}
		virtual void fill_selector(Selector* selector) {}

		bool is_dirty();
		void mark_dirty() { dirty = true; }

	protected:
		void set_scroll_range(size_t s);
		size_t get_scroll_range() { return scroll_range; }
		void set_centered_height(size_t h);
		int x_offset() { return offset.x; }
		int y_offset() { return offset.y; }
		inline Selection* selection() { return state->selection; }
		
		//pointer to the display's shared state
		//(contains the current Selection*)
		State* const state;

	private:

		//does this widget need rendering
		bool dirty = true;

		// offset.x --> used for horizontal scrolling
		// offset.y --> used for vertically centering the row of files
		SDL_Point offset = {0, 0};
		size_t scroll_range = 0;
};
