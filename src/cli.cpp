

#include <SDL.h>
#include <SDL_ttf.h>
#include <string.h>
#include "cli.h"
#include "collector.h"
#include "texture.h"


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
}

void CLI::render()
{
	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

	SDL_Rect background;
	

	int win_w = 0;
	int win_h = 0;
	SDL_GetWindowSize(window, &win_w, &win_h);

	background.x = 0;
	background.y = win_h - 20;
	background.w = win_w;
	background.h = 20;

	SDL_RenderFillRect(renderer, &background);

	if(text.length() > 0)
	{
		SDL_Color color = {255,255,255,255};
		Texture* texture = new Texture;
		texture->load_text(text, color);
		texture->render(4, win_h - 16);
		delete texture;
	}
}
