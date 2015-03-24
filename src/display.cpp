

#include <string>

#include <SDL.h>

#include "collector.h"
#include "config.h"
#include "display.h"



Display::Display()
{
	cli = new CLI;
}

Display::~Display()
{
	delete cli;
}

void Display::on_resize()
{
	SDL_GetWindowSize(window,
					  &(config->window_w),
					  &(config->window_h));
}

void Display::on_key(SDL_KeyboardEvent &e)
{

	switch(e.keysym.sym)
	{
		case SDLK_ESCAPE:
			send_quit();
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

void Display::render()
{
	cli->render();	
	/*
	int x = 1;
	int y = 1;
	int inc = 6;

	setRenderDrawColor(renderer, config->get_color(FILL));

	for(File* file: filestore->get_files())
	{
		file->rect = { x, y, 5, 5 };

		SDL_RenderFillRect(renderer, &(file->rect));

		x += inc;
		if(x > config->window_w)
		{
			x = 1;
			y += inc;
		}
	}
	*/
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