
#pragma once

#include <string>

#include "SDL.h"

#include <filestore/file.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>

class Info : public DisplayObject
{
	public:
		Info(State* s);
		~Info();

		void render();
		void on_state_change();

	private:
		Text* filepath;
		Text* filesize;
};
