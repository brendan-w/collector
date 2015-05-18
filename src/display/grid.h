
#pragma once

#include <vector>
#include <unordered_map>

#include <SDL.h>

#include <text.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <filestore/file.h>
#include <filestore/selection.h>


typedef struct {
	size_t x; //column location for this tag
	Text* text;
} Grid_Tag;


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

		//tag names below the grid
		std::vector<Grid_Tag> tags;

		//highlighted regions of the minimap
		std::vector<Bound> minimap;

		//used to prevent excessive recalculation during window resize
		size_t current_height_files = 0;
		size_t current_width_files = 0;

		void render_minimap();
		void render_tags();
		void update_minimap();
		void auto_scroll();
		void render_file(File* file, bool selected);
		File* mouse_to_file(int x, int y);
		size_t create_tag(File* file, size_t column);
		void clear_tags();
};
