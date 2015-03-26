
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <string>
#include <vector>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <utils.h>
#include <text.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/cli.h>


CLI::CLI()
{
	new_tag(); //create the initial empty tag field
	totals = new Text("", config->get_color(CLI_TEXT));
}


CLI::~CLI()
{
	destroy_tags();
	delete totals;
}


bool CLI::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_BACKSPACE:
			return backspace();
		case SDLK_DELETE:
			return delete_tag();
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

	return false;
}


bool CLI::on_text(SDL_TextInputEvent &e)
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

		return false;
	}
	else
	{
		Text* t = current_tag();
		t->set_text(t->get_text() += e.text);
		return true;
	}

}


void CLI::render()
{
	//draw the background
	setRenderDrawColor(renderer, config->get_color(OVERLAY));
	SDL_RenderFillRect(renderer, &rect);

	totals->render(rect.w - totals->width(),
				   rect.y + config->CLI_padding);

	render_tags();
}



void CLI::render_tags()
{
	SDL_Rect tag_rect = rect;

	//draw each tags text
	setRenderDrawColor(renderer, config->get_color(CLI_HIGHLIGHT));

	int x = config->CLI_padding;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		Text* t = tags[i];
		
		if(i == current)
		{
			tag_rect.x = x - config->CLI_padding;
			tag_rect.w = t->width() + (config->CLI_padding * 2);
			SDL_RenderFillRect(renderer, &tag_rect);
		}

		t->render(x, rect.y + config->CLI_padding);
		x += t->width() + (config->CLI_padding * 2);
	}
}

void CLI::layout()
{
	rect = {
		0,
		config->window.h - config->CLI_height,
		config->window.w,
		config->CLI_height
	};
}


void CLI::fill_selector(Selector* selector)
{
	//dump our tags into the selector
	for(Text* t: tags)
	{
		selector->add_operation(t->get_text());
	}
}


void CLI::read_selection(Selection* selection)
{
	//update the internal state
	std::string s = "";
	s += int_to_str(selection->size());
	s += " / ";
	s += int_to_str(selection->all_size());

	totals->set_text(s);
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
bool CLI::delete_tag()
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
		return true;
	}
	else if(t->get_text().length() > 0)
	{
		//if the user deleted the last tag, simply empty it
		t->set_text("");
		return true;
	}

	return false;
}

bool CLI::backspace()
{
	std::string s = current_tag()->get_text();
	if(s.length() > 0)
	{
		s.pop_back();
		current_tag()->set_text(s);
		return true;
	}

	return false;
}
