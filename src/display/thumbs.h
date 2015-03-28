
#pragma once

#include <SDL.h>

#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selection.h>


class Thumbs : public virtual DisplayObject
{
	public:
		Thumbs(Selection** s);
		~Thumbs();
		void render();
		void layout(bool force);

		bool on_motion(SDL_MouseMotionEvent &e);

		void on_selection();

	private:
		int width; //width of column in number of files

		void render_file(File* file);
		File* mouse_to_file(int x, int y);
};
