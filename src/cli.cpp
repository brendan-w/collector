

#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include <iostream>
#include "cli.h"


CLI::CLI()
{
	SDL_StartTextInput();
	dirty = true;
}

CLI::~CLI()
{
	SDL_StopTextInput();
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

}
