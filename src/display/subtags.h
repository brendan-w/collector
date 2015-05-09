
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/selection.h>
#include <display/displayobject.h>
#include <text.h>

class Subtags : public DisplayObject
{
	public:
		Subtags(Selection** s);
		~Subtags();

		void render();
		void on_selection();

	private:
		std::vector<Text*> tags;

		void render_tags();

		void new_tag();
};
