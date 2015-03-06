

#include <string.h>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "collector.h"
#include "text.h"
#include "cli.h"




/*
	Main CLI logic
*/


CLI::CLI()
{
	//create the initial empty tag field
	new_tag();
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

Text* CLI::current_tag()
{
	return tags[current];
}

void CLI::new_tag()
{
	Text* t = new Text("", config->get_highlight_color());
	tags.push_back(t);

	current = tags.size() - 1;
}


void CLI::handle_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_BACKSPACE:
		{
			std::string s = current_tag()->get_text();
			if(s.length() > 0)
			{
				s.pop_back();
				current_tag()->set_text(s);
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
		new_tag();
	}
	else
	{
		Text* t = current_tag();
		t->set_text(t->get_text() += e.text);
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
		Text* t = tags[i];
		t->render(x, win_h - 16);
		x += t->width() + 10;
	}
}


void CLI::send_quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;

	//SDL copies memory into event queue, so this is memory safe
	SDL_PushEvent(&e);
}
