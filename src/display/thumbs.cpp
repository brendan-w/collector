

#include <iostream>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <thumbnail.h>
#include <event.h>
#include <utils.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/thumbs.h>



Thumbs::Thumbs(Selection** s) : DisplayObject(s)
{

}

Thumbs::~Thumbs()
{

}

void Thumbs::render()
{
	context->setColor(config->get_color(FILE_NEUTRAL));

	for(File* file : *get_selection())
	{
		render_file(file);
	}
}


void Thumbs::render_file(File* file)
{
	SDL_Rect rect = {
		file->point.x + x_offset(), //adjust position for centering
		file->point.y + y_offset(), //adjust position for scroll
		FILE_THUMB_SIZE,
		FILE_THUMB_SIZE
	};

	if(context->rectInWindow(rect))
	{
		file->load();
		file->get_thumb()->render(&rect);
	}
}


//generates a raster column of files
//updates every File->point
void Thumbs::layout(bool force)
{
	rect = config->get_window_rect();

	//compute how many file thumbnails will fit in the window
	const int new_width = (int) floor( (double)(WINDOW_W / FILE_THUMB_OFFSET) );

	//calculate the offset necessary to horizontally center the column
	set_centered_width(new_width * FILE_THUMB_OFFSET);

	const int num_rows = (int) ceil( (double)get_selection()->size() / new_width );

	set_scroll_height(num_rows * FILE_THUMB_OFFSET);

	//prevent excessive recomputation of the layout
	if(force || (width != new_width))
	{
		width = new_width;

		int i = 0;

		for(File* file : *get_selection())
		{
			file->point = {
				(i % width) * FILE_THUMB_OFFSET,
				(i / width) * FILE_THUMB_OFFSET
			};

			i++;
		}
	}
}

bool Thumbs::on_wheel(SDL_MouseWheelEvent &e)
{
	DisplayObject::on_wheel(e);

	// for(File* file : *get_selection())
	// {

	// }

	return false;
}

void Thumbs::on_selection()
{
	layout(true);
}

bool Thumbs::on_motion(SDL_MouseMotionEvent &e)
{
	File* file = mouse_to_file(e.x, e.y);
	submit(FILE_INFO, (void*) file);
	return false;
}

//lookup the file under the cursor
File* Thumbs::mouse_to_file(int x, int y)
{
	//adjust for view offsets (scrolling & centering)
	SDL_Point m = {
		x - x_offset(),
		y - y_offset(),
	};

	//prevents negative numbers from reaching the division
	//save us from having to do a floor()
	if(m.x < 0)
		return NULL;

	//translate into H curve coordinate space
	m = {
		m.x / FILE_THUMB_OFFSET,
		m.y / FILE_THUMB_OFFSET
	};

	if((m.x < width) && (m.y >= 0))
	{
		size_t d = m.x + (m.y * width);
		if(d < get_selection()->size())
		{
			return get_selection()->at(d);
		}
	}

	return NULL;
}
