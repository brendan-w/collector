
#pragma once

#include <vector>

#include <SDL.h>

#include <display/displayobject.h>
#include <filestore/file.h>
#include <filestore/selection.h>


typedef struct {
	int lower;
	int upper;
} Bounds;


class Grid : public DisplayObject
{
	public:
		Grid(Selection** s);
		~Grid();
		void render();
		void resize();

		void on_selection();
		void on_wheel(SDL_MouseWheelEvent &e);
		void on_motion(SDL_MouseMotionEvent &e);

	private:

		//highlighted regions of the minimap
		std::vector<Bounds> minimap;

		SDL_Point mouse;
		File* file_under_mouse = NULL;

		//used to prevent excessive recalculation during window resize
		size_t current_height_files = 0;
		size_t current_width_files = 0;

		void render_file(File* file, bool selected);
		void update_hover();
		File* mouse_to_file(int x, int y);
};
