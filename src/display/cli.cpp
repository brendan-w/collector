
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <iostream>
#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/cli_tags.h>
#include <display/cli_command.h>
#include <display/cli.h>


#define TAG_MODE_PROMPT "SELECT"
#define TAG_MODE_COLOR FILE_INCLUDED
#define COMMAND_MODE_PROMPT "COMMAND"
#define COMMAND_MODE_COLOR FILE_INCLUDED




CLI::CLI(State* s) : DisplayObject(s)
{
	prompt   = new Text("", config->get_color(HIGHLIGHT));
	totals   = new Text("", config->get_color(CLI_LIGHT));
	includes = new Text("", config->get_color(CLI_DARK));
	excludes = new Text("", config->get_color(CLI_DARK));

	tags    = new CLI_Tags(s);
	command = new CLI_Command(s);

	set_mode(tags);
	on_selection();    //set the selection totals
	on_state_change(); //set the include/exclude totals
}


CLI::~CLI()
{
	delete prompt;
	delete totals;
	delete includes;
	delete excludes;

	delete tags;
	delete command;
}

void CLI::carry_dirt()
{
	if(current->is_dirty())
		mark_dirty();
}

bool CLI::on_key(SDL_KeyboardEvent &e)
{
	if(state->key_ctrl)
	{
		switch(e.keysym.sym)
		{
			case SDLK_TAB:
				cycle_mode();
				return false;

			case SDLK_i:
				state->clear_includes();
				sdl->submit(STATE_CHANGE);
				mark_dirty();
				return true;

			case SDLK_e:
				state->clear_excludes();
				sdl->submit(STATE_CHANGE);
				mark_dirty();
				return true;

			case SDLK_a:
				for(File* f: *selection())
					state->inexclude_file(f, true);
				mark_dirty();
				return true;
		}

	}
	else
	{
		bool r = current->on_key(e);
		carry_dirt();
		return r;
	}

	return false;
}


void CLI::on_text(SDL_TextInputEvent &e)
{
	current->on_text(e);
	carry_dirt();
}


void CLI::render()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw the background
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	const int y = rect.y + CLI_PAD;
	int rx = 0;

	//right aligned content
	rx = rect.w - CLI_PAD;

	rx -= totals->width();
	totals->render(rx, y);

	rx -= (CLI_PAD * 6);

	rx -= excludes->width();
	excludes->render(rx, y);

	rx -= (CLI_PAD * 2);

	rx -= includes->width();
	includes->render(rx, y);


	//left aligned content

	int lx = CLI_PAD;
	prompt->render(lx, y);
	lx += prompt->width() + CLI_PAD;

	SDL_Rect content = {
		lx,
		0,
		rx - lx,
		rect.h
	};

	sdl->set_viewport(content);

	current->render();
}


void CLI::fill_selector(Selector* selector)
{
	//the Tags component is the only one with selector data
	tags->fill_selector(selector);
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

	current->on_selection();

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

	current->on_state_change();

	mark_dirty();
}

void CLI::cycle_mode()
{
	if(current == tags)
		set_mode(command);
	else if(current == command)
		set_mode(tags);
}

void CLI::set_mode(DisplayObject* m)
{
	current = m;

	if(current == tags)
	{
		prompt->set_text(TAG_MODE_PROMPT);
		prompt->set_color(config->get_color(TAG_MODE_COLOR));
	}
	else if(current == command)
	{
		prompt->set_text(COMMAND_MODE_PROMPT);
		prompt->set_color(config->get_color(COMMAND_MODE_COLOR));
	}

	mark_dirty();
}
