

#include <string>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <event.h>
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
	info = new Info(&selection);
	cli = new CLI(&selection);

	//add all the different types of main displays
	views.push_back(new Grid(&selection));
	views.push_back(new Thumbs(&selection));

	//set the default display
	current_view = 0;

	//trigger the initial layout
	on_resize();
}

Display::~Display()
{
	delete cli;
	delete info;

	for(DisplayObject* v: views)
		delete v;

	views.clear();

	if(selection != NULL)
		delete selection;
}

void Display::render()
{
	view->render();
	info->render();
	cli->render();
}

void Display::on_resize()
{
	SDL_GetWindowSize(context->get_window(),
					  &(config->window.w),
					  &(config->window.h));

	view->layout(false);
	info->layout(false);
	cli->layout(false);

	//in case layout() never adjusts/handles the scroll
	view->limit_scroll();
}

void Display::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_ESCAPE:
			send_quit();
			break;
		case SDLK_RETURN:
			break;
		case SDLK_TAB:
			cycle_view();
			/*
				Force the layout to be recomputed, since either the window
				may have changed around it, or the files are in positions
				for a different view
			*/
			view->layout(true);
			break;
		case SDLK_PAGEUP:
			view->pageup();
			break;
		case SDLK_PAGEDOWN:
			view->pagedown();
			break;
		default:
			if(cli->on_key(e))
				send_selector();
			break;
	}
}

void Display::on_text(SDL_TextInputEvent &e)
{
	if(cli->on_text(e))
		send_selector();
}

void Display::on_wheel(SDL_MouseWheelEvent &e)
{
	if(view->on_wheel(e))
		send_selector();
}

void Display::on_motion(SDL_MouseMotionEvent &e)
{
	if(view->on_motion(e))
		send_selector();
}

void Display::on_selection(Selection* new_selection)
{
	//replace the current selection
	if(selection != NULL)
		delete selection;
	selection = new_selection;

	//let the components update themselves
	view->on_selection();
	cli->on_selection();
	info->on_selection();
}

void Display::on_file_info(File* f)
{
	info->on_file_info(f);
}

void Display::cycle_view()
{
	current_view++;
	current_view = current_view % views.size();
}

void Display::send_quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;

	//SDL copies memory into event queue, so this is memory safe
	SDL_PushEvent(&e);
}

void Display::send_selector()
{
	Selector* selector = new Selector;

	//ask the various components for their data
	cli->fill_selector(selector);

	//send to SDL event queue
	submit(SELECTOR, (void*) selector);
}
