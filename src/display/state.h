
#pragma once

#include <filestore/types.h>

/*
	A place to save state shared between DisplayObjects
*/

class State
{
	public:
		State();
		~State();

		Selection* selection = NULL;    //the current file selection returned from the filestore
		File* file_under_mouse = NULL;  //pointer to the file currently under the mouse
		file_map_bool inexclude;        //manual include/exclude table
		bool should_autoscroll = false; //whether or not the most recent selection should be autoscrolled
		bool key_ctrl = false;         //whether a shift key is depressed

		void replace_selection(Selection* s);
		void fill_selector(Selector* s);
		void toggle_inexclude(File* f, bool include);
		bool inexclude_has(File* f);
		void clear_includes();
		void clear_excludes();

	private:
		void remove_inexclude_by_val(bool val);
};
