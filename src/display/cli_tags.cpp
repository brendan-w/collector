
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# cli_tags.cpp                                                         #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/


#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/cli_tags.h>



CLI_Tag::CLI_Tag()
{
	text       = new Text("", config->get_color(CLI_LIGHT));
	completion = new Text("", config->get_color(CLI_DARK));
}

CLI_Tag::~CLI_Tag()
{
	delete text;
	delete completion;
}

Tag CLI_Tag::get_tag()
{
	if(get_op() == EXCLUSION)
		return get_t().substr(1);

	return get_t();
}

Set_Operation CLI_Tag::get_op()
{
	if(len_t() > 0)
	{
		switch(get_t().at(0))
		{
			case '-':
				return EXCLUSION;
				break;
		}		
	}

	return INTERSECTION;
}






CLI_Tags::CLI_Tags(State* s) : DisplayObject(s)
{
	new_tag(); //create the initial empty tag field
	on_selection();
	on_state_change();
}


CLI_Tags::~CLI_Tags()
{
	destroy_tags();
}


bool CLI_Tags::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_RETURN:
			state->selection->export_();
			break;
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


void CLI_Tags::on_text(SDL_TextInputEvent &e)
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
		CLI_Tag* t = current_tag();
		t->set_t(t->get_t() + e.text);
		state->should_autoscroll = true;
		mark_dirty();
	}
}


void CLI_Tags::render()
{
	SDL_Rect rect = sdl->get_viewport();
	SDL_Rect tag_rect = rect;

	//draw each tags text
	sdl->set_color(HIGHLIGHT);

	int x = CLI_PAD;
	const int y = rect.y + CLI_PAD;

	for(size_t i = 0; i < tags.size(); i++)
	{
		CLI_Tag* t = tags[i];
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

void CLI_Tags::fill_selector(Selector* selector)
{
	//dump our tags into the selector
	for(CLI_Tag* t: tags)
	{
		selector->add_tag(t->get_tag(), t->get_op());
	}
}


void CLI_Tags::on_selection()
{
	Selection* s = selection();

	//update the status of the current tag
	CLI_Tag* t = current_tag();
	Tag tag = t->get_tag();
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

//deallocates all Text objects in tags
void CLI_Tags::destroy_tags()
{
	for(CLI_Tag* t: tags)
		delete t;

	tags.clear();
}


CLI_Tag* CLI_Tags::current_tag()
{
	return tags[current_index];
}


//creates a new tag at the end of the tags vector
//and sets it as the current
void CLI_Tags::new_tag()
{
	CLI_Tag* t = new CLI_Tag();
	tags.push_back(t);
	current_index = tags.size() - 1;
}


//deletes the current tag, ensures that there is
//always one tag in the vector
void CLI_Tags::delete_tag()
{
	CLI_Tag* t = current_tag();

	//ensure that there is always at least one tag preset
	if(tags.size() > 1)
	{
		delete t;
		tags.erase(tags.begin() + current_index);

		if(current_index > (tags.size() - 1))
			current_index--;

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

void CLI_Tags::backspace()
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

void CLI_Tags::auto_complete()
{
	CLI_Tag* t = current_tag();

	t->set_t(t->get_t() + t->get_c());
	t->set_c("");
	state->should_autoscroll = true;
	mark_dirty();
}
