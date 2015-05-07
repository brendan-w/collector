

#include <string>

#include <SDL.h>

#include <collector.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/cli.h>
#include <display/info.h>
#include <display/grid.h>
#include <display/thumbs.h>
#include <display/display.h>





Display::Display(Selection* init_selection)
{
	selection = init_selection;

	//create the main components, with references to the displays state
	cli.display    = new CLI(&selection);
	info.display   = new Info(&selection);
	grid.display   = new Grid(&selection);
	// thumbs.display = new Thumbs(&selection);

	//trigger the initial layout
	resize();
}

Display::~Display()
{
	delete cli.display;
	delete info.display;
	delete grid.display;
	// delete thumbs.display;

	if(selection != NULL)
		delete selection;
}

void Display::render()
{
	// render_child(thumbs);
	render_child(grid);
	render_child(info);
	render_child(cli);

	sdl->reset_viewport();
}

void Display::render_child(Child& child)
{
	if(child.display->is_dirty())
	{
		sdl->set_viewport(child.rect);
		child.display->render();
	}
}

void Display::resize()
{
	SDL_Point window = sdl->window_size();
	SDL_Point middle = {
		(window.x > 0) ? (window.x / 2) : 0,
		(window.y > 0) ? (window.y / 2) : 0,
	};

	/*
		Layout information
		Some day, this should be user configurable
	*/

	cli.rect = {
		0,
		window.y - CLI_H,
		window.x,
		CLI_H
	};

	info.rect = {
		0,
		0,
		window.x,
		CLI_H
	};

	grid.rect = {
		0,
		CLI_H,
		window.x,
		middle.y - CLI_H
	};

	// resize_child(thumbs);
	resize_child(grid);
	resize_child(info);
	resize_child(cli);

	//in case layout() never adjusts/handles the scroll
	// view->limit_scroll();
}

void Display::resize_child(Child& child)
{
	sdl->set_viewport(child.rect);
	child.display->resize();
}

void Display::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_ESCAPE:
			sdl->quit();
			break;
		case SDLK_RETURN:
			break;
		case SDLK_TAB:
			break;
		case SDLK_PAGEUP:
			break;
		case SDLK_PAGEDOWN:
			break;
		default:
			if(cli.display->on_key(e))
				send_selector();
			break;
	}
}

void Display::on_text(SDL_TextInputEvent &e)
{
	if(cli.display->on_text(e))
		send_selector();
}

void Display::on_wheel(SDL_MouseWheelEvent &e)
{
	// if(view->on_wheel(e))
	// 	send_selector();
}

void Display::on_motion(SDL_MouseMotionEvent &e)
{
	// if(view->on_motion(e))
	// 	send_selector();
}

void Display::on_selection(Selection* new_selection)
{
	//replace the current selection
	if(selection != NULL)
		delete selection;
	selection = new_selection;

	//let the components update themselves
	cli.display->on_selection();
	info.display->on_selection();
}

void Display::on_file_info(File* f)
{
	info.display->on_file_info(f);
}

void Display::send_selector()
{
	Selector* selector = new Selector;

	//ask the various components for their data
	cli.display->fill_selector(selector);

	//send to SDL event queue
	sdl->submit(SELECTOR, (void*) selector);
}
