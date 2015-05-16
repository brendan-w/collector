
#pragma once

#include <vector>
#include <unordered_map>

#include <SDL.h>

#include <display/displayobject.h>
#include <display/state.h>
#include <filestore/file.h>
#include <filestore/selection.h>


typedef struct {
	double lower;
	double upper;
} Bound;


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
		std::vector<Bound> minimap;

		//used to prevent excessive recalculation during window resize
		size_t current_height_files = 0;
		size_t current_width_files = 0;

		void render_minimap();
		void update_minimap();
		void render_file(File* file, bool selected);
		File* mouse_to_file(int x, int y);
};
