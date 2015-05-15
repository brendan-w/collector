
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/cli.h>



Tag::Tag()
{
	text       = new Text("", config->get_color(CLI_LIGHT));
	completion = new Text("", config->get_color(CLI_DARK));
	valid      = true;
}

Tag::~Tag()
{
	delete text;
	delete completion;
}

std::string Tag::get_text()
{
	return text->get_text();
}

void Tag::set_text(const std::string & t)
{
	text->set_text(t);
}

std::string Tag::get_completion()
{
	return completion->get_text();
}

void Tag::set_completion(const std::string & t)
{
	completion->set_text(t);
}






CLI::CLI(State* s) : DisplayObject(s)
{
	new_tag(); //create the initial empty tag field
	totals = new Text("", config->get_color(CLI_LIGHT));
	on_selection();
}


CLI::~CLI()
{
	destroy_tags();
	delete totals;
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
			auto_complete();
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		case SDLK_LEFT:
			if(current_index > 0)
			{
				current_index--;
				mark_dirty();
			}
			break;
		case SDLK_RIGHT:
			if(current_index < (tags.size() - 1))
			{
				current_index++;
				mark_dirty();
			}
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
			current_index = tags.size() - 1;
		}
		else
		{
			new_tag();
			mark_dirty();
		}
	}
	else
	{
		Tag* t = current_tag();
		t->set_text(t->get_text() += e.text);
		mark_dirty();
	}
}


void CLI::render()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw the background
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	totals->render(rect.w - totals->width() - CLI_PAD,
				   rect.y + CLI_PAD);

	render_tags();
}



void CLI::render_tags()
{
	SDL_Rect rect = sdl->get_viewport();
	SDL_Rect tag_rect = rect;

	//draw each tags text
	sdl->set_color(HIGHLIGHT);

	int x = CLI_PAD;
	const int y = rect.y + CLI_PAD;

	for(size_t i = 0; i < tags.size(); i++)
	{
		Tag* t = tags[i];
		int tw = t->text->width();
		int cw = t->completion->width();

		tag_rect.x = x - CLI_PAD;
		tag_rect.w = tw + (CLI_PAD * 2);
		
		if(i == current_index)
		{
			tag_rect.w += cw;
			sdl->fill_rect(tag_rect);
		}

		t->text->render(x, y);

		if(i == current_index)
		{
			t->completion->render(x + tw, y);
		}

		x += tag_rect.w;
	}
}

void CLI::fill_selector(Selector* selector)
{
	//dump our tags into the selector
	for(Tag* t: tags)
	{
		selector->add_operation(t->get_text(), INTERSECTION);
	}
}


void CLI::on_selection()
{
	Selection* s = selection();

	//update the internal state
	std::string totals_str = "";
	totals_str += std::to_string(s->size());
	totals_str += " / ";
	totals_str += std::to_string(s->all_size());
	totals->set_text(totals_str);

	//update the status of the current tag
	Tag* t = current_tag();
	std::string tag = t->get_text();
	if(s->has_subtag(tag))
	{
		t->text->set_color(config->get_color(CLI_LIGHT));
		t->set_completion("");
	}
	else
	{
		t->text->set_color(config->get_color(CLI_ERROR));
		if(tag.length() > 0)
			t->set_completion(s->auto_complete(tag));
		else
			t->set_completion("");
	}

	mark_dirty();
}


//deallocates all Text objects in tags
void CLI::destroy_tags()
{
	for(Tag* tag: tags)
		delete tag;

	tags.clear();
}


Tag* CLI::current_tag()
{
	return tags[current_index];
}


//creates a new tag at the end of the tags vector
//and sets it as the current
void CLI::new_tag()
{
	Tag* t = new Tag();
	tags.push_back(t);
	current_index = tags.size() - 1;
}


//deletes the current tag, ensures that there is
//always one tag in the vector
void CLI::delete_tag()
{
	Tag* t = current_tag();

	//ensure that there is always at least one tag preset
	if(tags.size() > 1)
	{
		delete t;
		tags.erase(tags.begin() + current_index);

		if(current_index > (tags.size() - 1))
		{
			current_index--;
		}
		mark_dirty();
	}
	else if(t->get_text().length() > 0)
	{
		//if the user deleted the last tag, simply empty it
		t->set_text("");
		t->set_completion("");
		mark_dirty();
	}
}

void CLI::backspace()
{
	std::string s = current_tag()->get_text();
	if(s.length() > 0)
	{
		s.pop_back();
		current_tag()->set_text(s);
		mark_dirty();
	}
}

void CLI::auto_complete()
{
	Tag* t = current_tag();

	t->set_text(t->get_text() + t->get_completion());
	t->set_completion("");
	mark_dirty();
}
