
#pragma once

#include <vector>
#include <unordered_map>

#include <SDL.h>

#include <display/displayobject.h>
#include <display/state.h>
#include <filestore/file.h>
#include <filestore/selection.h>


typedef struct {
	int lower;
	int upper;
} Bounds;


class Grid : public DisplayObject
{
	public:
		Grid(State* s);
		~Grid();
		void render();
		void resize();

		void on_selection();
		void on_wheel(SDL_MouseWheelEvent &e);
		void on_click(SDL_MouseButtonEvent &e);
		void on_motion(SDL_MouseMotionEvent &e);

	private:

		//highlighted regions of the minimap
		std::vector<Bounds> minimap;

		SDL_Point mouse;

		//used to prevent excessive recalculation during window resize
		size_t current_height_files = 0;
		size_t current_width_files = 0;

		void render_file(File* file, bool selected);
		void update_hover();
		File* mouse_to_file(int x, int y);
};
