
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
}

Tag::~Tag()
{
	delete text;
	delete completion;
}





CLI::CLI(State* s) : DisplayObject(s)
{
	new_tag(); //create the initial empty tag field
	totals   = new Text("", config->get_color(CLI_LIGHT));
	includes = new Text("", config->get_color(CLI_DARK));
	excludes = new Text("", config->get_color(CLI_DARK));
	on_selection();
	on_state_change();
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
			backspace();
			return true;
		case SDLK_DELETE:
			delete_tag();
			return true;
		case SDLK_TAB:
			auto_complete();
			return true;
		case SDLK_UP:
			return true;
		case SDLK_DOWN:
			return true;
		case SDLK_LEFT:
			if(current_index > 0)
			{
				current_index--;
				mark_dirty();
			}
			return true;
		case SDLK_RIGHT:
			if(current_index < (tags.size() - 1))
			{
				current_index++;
				mark_dirty();
			}
			return true;
		default:
			break;
	}

	return false;
}


void CLI::on_text(SDL_TextInputEvent &e)
{
	//space bar starts a new tag
	if(e.text[0] == ' ')
	{
		//if the last tag is empty, skip to it (rather than adding another)
		if(tags[tags.size() - 1]->len_t() == 0)
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
		t->set_t(t->get_t() += e.text);
		state->should_autoscroll = true;
		mark_dirty();
	}
}


void CLI::render()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw the background
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	int x = rect.w - CLI_PAD;
	const int y = rect.y + CLI_PAD;

	x -= totals->width();
	totals->render(x, y);

	x -= (CLI_PAD * 6);

	x -= excludes->width();
	excludes->render(x, y);

	x -= (CLI_PAD * 2);

	x -= includes->width();
	includes->render(x, y);

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
		selector->add_operation(t->get_t(), INTERSECTION);
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
	std::string tag = t->get_t();
	if(s->has_subtag(tag))
	{
		t->text->set_color(config->get_color(CLI_LIGHT));
		t->set_c("");
	}
	else
	{
		t->text->set_color(config->get_color(CLI_ERROR));
		if(tag.length() > 0)
			t->set_c(s->auto_complete(tag));
		else
			t->set_c("");
	}

	mark_dirty();
}


void CLI::on_state_change()
{
	//update the include/exclude counts
	size_t n_includes = 0;
	size_t n_excludes = 0;

	//sum up them up
	for(auto e: state->inexclude)
	{
		if(e.second) n_includes++;
		else         n_excludes++;
	}

	includes->set_text("+" + std::to_string(n_includes));
	if(n_includes == 0) includes->set_color(config->get_color(CLI_DARK));
	else                includes->set_color(config->get_color(FILE_INCLUDED));

	excludes->set_text("-" + std::to_string(n_excludes));
	if(n_excludes == 0) excludes->set_color(config->get_color(CLI_DARK));
	else                excludes->set_color(config->get_color(FILE_EXCLUDED));

	mark_dirty();
}

//deallocates all Text objects in tags
void CLI::destroy_tags()
{
	for(Tag* t: tags)
		delete t;

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

		state->should_autoscroll = true;
		mark_dirty();
	}
	else if(t->len_t() > 0)
	{
		//if the user deleted the last tag, simply empty it
		t->set_t("");
		t->set_c("");
		state->should_autoscroll = true;
		mark_dirty();
	}
}

void CLI::backspace()
{
	std::string s = current_tag()->get_t();
	if(s.length() > 0)
	{
		s.pop_back();
		current_tag()->set_t(s);
		state->should_autoscroll = true;
		mark_dirty();
	}
}

void CLI::auto_complete()
{
	Tag* t = current_tag();

	t->set_t(t->get_t() + t->get_c());
	t->set_c("");
	state->should_autoscroll = true;
	mark_dirty();
}
