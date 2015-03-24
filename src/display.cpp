

#include <string>

#include <SDL.h>

#include "collector.h"
#include "config.h"
#include "file.h"
#include "cli.h"
#include "grid.h"
#include "display.h"



Display::Display()
{
	cli = new CLI;
	grid = new Grid;
}

Display::~Display()
{
	delete cli;
	delete grid;
}

void Display::on_resize(file_list::iterator begin, file_list::iterator end)
{
	SDL_GetWindowSize(window,
					  &(config->window_w),
					  &(config->window_h));

	grid->layout(begin, end);
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
			cli->on_key(e);
	}

	send_selector();
}

void Display::on_text(SDL_TextInputEvent &e)
{
	cli->on_text(e);
	send_selector();
}

void Display::on_wheel(SDL_MouseWheelEvent &e)
{
	grid->on_wheel(e);
}

void Display::render(file_list::iterator begin, file_list::iterator end)
{
	grid->render(begin, end);
	cli->render();
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

}
