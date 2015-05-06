
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <string>
#include <vector>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <text.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/cli.h>


CLI::CLI(Selection** s) : DisplayObject(s)
{
	new_tag(); //create the initial empty tag field
	totals = new Text("", config->get_color(CLI_TEXT));
	on_selection();
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
			if(current_index > 0)
				current_index--;
			break;
		case SDLK_RIGHT:
			if(current_index < (tags.size() - 1))
				current_index++;
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
			current_index = tags.size() - 1;
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
	context->set_color(OVERLAY);
	context->fill_rect(rect);

	totals->render(rect.w - totals->width() - CLI_PAD,
				   rect.y + CLI_PAD);

	render_tags();
}



void CLI::render_tags()
{
	SDL_Rect tag_rect = rect;

	//draw each tags text
	context->set_color(CLI_HIGHLIGHT);

	int x = CLI_PAD;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		Text* t = tags[i];
		
		if(i == current_index)
		{
			tag_rect.x = x - CLI_PAD;
			tag_rect.w = t->width() + (CLI_PAD * 2);
			context->fill_rect(tag_rect);
		}

		t->render(x, rect.y + CLI_PAD);
		x += t->width() + (CLI_PAD * 2);
	}
}

void CLI::layout(bool force)
{
	rect = {
		0,
		WINDOW_H - CLI_H,
		WINDOW_W,
		CLI_H
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


void CLI::on_selection()
{
	Selection* selection = get_selection();

	//update the internal state
	std::string s = "";
	s += std::to_string(selection->size());
	s += " / ";
	s += std::to_string(selection->all_size());

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
	return tags[current_index];
}


//creates a new tag at the end of the tags vector
//and sets it as the current
void CLI::new_tag()
{
	Text* t = new Text("", config->get_color(CLI_TEXT));
	tags.push_back(t);

	current_index = tags.size() - 1;
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
		tags.erase(tags.begin() + current_index);

		if(current_index > (tags.size() - 1))
		{
			current_index--;
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
