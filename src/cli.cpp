

#include <string.h>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "collector.h"
#include "texture.h"
#include "cli.h"



/*
	Individual Tag
*/

CLITag::CLITag()
{
	text = "";
	current_text = "";
	texture = new Texture;
}

CLITag::~CLITag()
{
	delete texture;
}

void CLITag::render(int x, int y)
{
	//only update the texture if the text has changed
	if(text != current_text)
	{
		texture->load_text(text, config->get_highlight_color());
		current_text = text;
	}

	texture->render(x, y);
}

int CLITag::width()
{
	return texture->width();
}




/*
	Main CLI logic
*/


CLI::CLI()
{
	current = 0;
	tags.push_back(new CLITag);
}


CLI::~CLI()
{
	destroy_tags();
}

void CLI::destroy_tags()
{
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		delete tags[i];
	}
	
	tags.clear();
}

CLITag* CLI::current_tag()
{
	return tags[current];
}


void CLI::handle_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_BACKSPACE:
		{
			CLITag* t = current_tag();
			if(t->text.length() > 0)
			{
				t->text.pop_back();
			}
			break;
		}
		case SDLK_RETURN:
			destroy_tags();
			break;
		case SDLK_ESCAPE:
			send_quit();
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		case SDLK_LEFT:
			break;
		case SDLK_RIGHT:
			break;
		default:
			break;
	}
}


void CLI::handle_text(SDL_TextInputEvent &e)
{
	//space bar starts a new tag
	if(e.text[0] == ' ')
	{
		tags.push_back(new CLITag);
		current = tags.size() - 1;
	}
	else
	{
		current_tag()->text += e.text;
	}
}


void CLI::render()
{
	//draw the main bar
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


	//draw each tags text
	int x = 4;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		CLITag* tag = tags[i];

		if(tag->text.length() > 0)
		{
			tag->render(x, win_h - 16);
			x += tag->width() + 10;
		}
	}
}


void CLI::send_quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;

	//SDL copies memory into event queue, so this is memory safe
	SDL_PushEvent(&e);
}
