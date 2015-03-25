
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <string>
#include <vector>

#include <SDL.h>

#include "collector.h"
#include "selector.h"
#include "config.h"
#include "text.h"
#include "cli.h"


CLI::CLI()
{
	new_tag(); //create the initial empty tag field
}


CLI::~CLI()
{
	destroy_tags();
}


void CLI::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_BACKSPACE:
			backspace();
			break;
		case SDLK_DELETE:
			delete_tag();
			break;
		case SDLK_TAB:
			//autocomplete
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


void CLI::on_text(SDL_TextInputEvent &e)
{
	//space bar starts a new tag
	if(e.text[0] == ' ')
	{
		//if the last tag is empty, skip to it (rather than adding another)
		if(tags[tags.size() - 1]->get_text().length() == 0)
		{
			current = tags.size() - 1;
		}
		else
		{
			new_tag();
		}
	}
	else
	{
		Text* t = current_tag();
		t->set_text(t->get_text() += e.text);
	}
}


void CLI::render()
{
	//draw the background
	setRenderDrawColor(renderer, config->get_color(OVERLAY));

	SDL_Rect current_rect = {
		0,
		config->window_h - config->CLI_height,
		config->window_w,
		config->CLI_height
	};

	SDL_RenderFillRect(renderer, &current_rect);


	//draw each tags text
	setRenderDrawColor(renderer, config->get_color(CLI_HIGHLIGHT));

	int x = config->CLI_padding;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		Text* t = tags[i];
		
		if(i == current)
		{
			current_rect.x = x - config->CLI_padding;
			current_rect.w = t->width() + (config->CLI_padding * 2);
			SDL_RenderFillRect(renderer, &current_rect);
		}

		t->render(x, config->window_h - 16);
		x += t->width() + (config->CLI_padding * 2);
	}
}


void CLI::fill_selector(Selector* selector)
{
	for(Text* t: tags)
	{
		selector->add_operation(t->get_text());
	}
}


void CLI::read_selection(Selection* selection)
{

}






//deallocates all Text objects in tags
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


//creates a new tag at the end of the tags vector
//and sets it as the current
void CLI::new_tag()
{
	Text* t = new Text("", config->get_color(CLI_TEXT));
	tags.push_back(t);

	current = tags.size() - 1;
}


//deletes the current tag, ensures that there is
//always one tag in the vector
void CLI::delete_tag()
{
	Text* t = current_tag();

	//ensure that there is always at least one tag preset
	if(tags.size() > 1)
	{
		delete t;
		tags.erase(tags.begin() + current);

		if(current > (tags.size() - 1))
		{
			current--;
		}
	}
	else
	{
		//if the user deleted the last tag, simply empty it
		t->set_text("");
	}
}

void CLI::backspace()
{
	std::string s = current_tag()->get_text();
	if(s.length() > 0)
	{
		s.pop_back();
		current_tag()->set_text(s);
	}
}
