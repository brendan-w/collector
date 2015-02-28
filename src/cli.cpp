

#include <string.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "collector.h"
#include "texture.h"
#include "cli.h"


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
		case SDLK_RETURN:
			text = "";
			break;
		case SDLK_ESCAPE:
			send_quit();
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
	setRenderDrawColor(renderer, config->get_fill_color());

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
		Texture* texture = new Texture;
		texture->load_text(text, config->get_highlight_color());
		texture->render(4, win_h - 16);
		delete texture;
	}
}


void CLI::send_quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;

	//SDL copies memory into event queue, so this is memory safe
	SDL_PushEvent(&e);
}
