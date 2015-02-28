

#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include "cli.h"
#include "collector.h"
#include "utils.h"


CLI::CLI()
{
	dirty = true;
}

CLI::~CLI()
{

}

void CLI::handle_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_BACKSPACE:
			if(text.length() > 0)
				text.pop_back();				
			break;
		default:
			break;
	}
}

void CLI::handle_text(SDL_TextInputEvent &e)
{
	text += e.text;
	dirty = true;
}

void CLI::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);	

	SDL_Rect a;
	a.x = 100;
	a.y = 100;
	a.w = 100;
	a.h = 100;

	SDL_RenderDrawRect(renderer, &a);



	if(dirty)
	{

		dirty = false;
	}
}
