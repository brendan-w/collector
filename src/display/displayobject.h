
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
		DisplayObject(Selection** s);
		virtual ~DisplayObject() {}

		virtual void render() {}
		virtual void layout(bool force=false) {}


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

	protected:
		Selection* get_selection() { return *selection; }

		void set_centered_width(int w);
		void set_scroll_height(int s);
		int x_offset();
		int y_offset();

		//area consumed, usually, the whole window

	private:
		//pointer to the current selection pointer
		Selection** selection;

		// offset.x --> used for centering the column of files
		// offset.y --> used for vertical scrolling
		SDL_Point offset = { 0, 0 };

		//the height of the entire scrollable region
		int scroll_height = 0;
};
