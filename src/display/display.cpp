

#include <string>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <event.h>
#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/cli.h>
#include <display/grid.h>
#include <display/display.h>



Display::Display(Selection* init_selection)
{
	//create the main components
	cli = new CLI;
	grid = new Grid;

	selection = init_selection;
}

Display::~Display()
{
	delete cli;
	delete grid;

	delete selection;
}

void Display::on_resize()
{
	SDL_GetWindowSize(window,
					  &(config->window.w),
					  &(config->window.h));

	grid->layout(selection);
	cli->layout();
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
	if(grid->on_wheel(e))
		send_selector();
}

void Display::render()
{
	grid->render(selection);
	cli->render();
}

void Display::on_selection(Selection* new_selection)
{
	//replace the current selection
	if(selection != NULL)
		delete selection;
	selection = new_selection;

	//let the components update themselves
	grid->read_selection(selection);
	cli->read_selection(selection);
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
