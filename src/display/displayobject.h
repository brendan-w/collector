
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# displayobject.h                                                      #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/

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
		virtual bool on_key(SDL_KeyboardEvent &e) { return false; } //returns bool for whether the key was accepted
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
		void scroll_to(int s);
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
