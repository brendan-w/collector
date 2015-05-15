
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

		Selection* selection = NULL;   //the current file selection returned from the filestore
		File* file_under_mouse = NULL; //pointer to the file currently under the mouse
		file_map_bool inexclude;       //manual include/exclude table

		void replace_selection(Selection* s);
		void fill_selector(Selector* s);
		bool inexclude_has(File* f);
};
