

#include <string.h>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>

#include "collector.h"
#include "text.h"
#include "cli.h"



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
		case SDLK_TAB:
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		case SDLK_LEFT:
			if(current > 0)
				current--;
			break;
		case SDLK_RIGHT:
			if(current < (tags.size() - 1))
				current++;
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

	SDL_Rect rect;

	int win_w = 0;
	int win_h = 0;
	SDL_GetWindowSize(window, &win_w, &win_h);

	rect.x = 0;
	rect.y = win_h - 20;
	rect.w = win_w;
	rect.h = 20;

	//draw each tags text
	int x = 4;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		Text* t = tags[i];
		
		if(i == current)
		{
			rect.x = x;
			rect.w = t->width();
			SDL_RenderFillRect(renderer, &rect);
		}

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
