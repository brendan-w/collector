
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# cli_command.cpp                                                      #
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


#include <iostream>
#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/cli_command.h>
#include <utils.h> //split()

#define MAX_HISTORY 50


CLI_Command::CLI_Command(State* s) : DisplayObject(s)
{
	command    = new Text("", config->get_color(CLI_LIGHT));
	completion = new Text("", config->get_color(CLI_DARK));
}


CLI_Command::~CLI_Command()
{
	delete command;
	delete completion;
	history.clear();
}


bool CLI_Command::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_RETURN:
			execute(); //will push an Operation onto the event queue
			break;
		case SDLK_BACKSPACE:
			backspace();
			break;
		case SDLK_TAB:
			//auto_complete();
			break;
		case SDLK_UP:

			if(history_index < (int)(history.size() - 1))
				history_index++;

			command->set_text(history[history_index]);

			mark_dirty();
			break;
		case SDLK_DOWN:

			if(history_index > -1)
				history_index--;

			if(history_index == -1)
				command->set_text(cmd);
			else
				command->set_text(history[history_index]);

			mark_dirty();
			break;
		case SDLK_LEFT:
			// if(current_index > 0)
			// {
			// 	current_index--;
			// 	mark_dirty();
			// }
			break;
		case SDLK_RIGHT:
			// if(current_index < (tags.size() - 1))
			// {
			// 	current_index++;
			// 	mark_dirty();
			// }
			break;
		default:
			break;
	}

	return false; //never trigger a selector
}


void CLI_Command::on_text(SDL_TextInputEvent &e)
{
	//get the text directly from the screen, in case the user is modifying a past command
	cmd = command->get_text() + e.text;
	command->set_text(cmd);

	mark_dirty();
}


void CLI_Command::render()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw each tags text
	sdl->set_color(HIGHLIGHT);

	int x = CLI_PAD;
	const int y = rect.y + CLI_PAD;

	command->render(x, y);
	x += command->width();
	completion->render(x, y);
}

void CLI_Command::execute()
{
	//get all commands, delimited by spaces
	std::string cmd_text = command->get_text();
	std::vector<std::string> cmds = split(cmd_text, " ");

	//new operation on this selections
	Operation* op = new Operation(selection());

	//parse load each command into the Operation
	for(std::string cmd: cmds)
		parse_command(op, cmd);

	//if the operation modifies files
	if(op->is_changing())
	{
		sdl->submit(OPERATION, op);
		sdl->submit(STATE_CHANGE); //update the file info readout
		post_execute();
	}

	mark_dirty();
}

void CLI_Command::parse_command(Operation* op, std::string & cmd)
{
	if(cmd.length() == 0)
		return;

	switch(cmd.at(0))
	{
		case '+': //add tag to selection
			op->add(ADD_TAG, cmd.substr(1));
			break;
		case '-': //remove tag from selection
			op->add(REMOVE_TAG, cmd.substr(1));
			break;
		default:
			break;
	}
}

void CLI_Command::post_execute()
{
	if(history.size() > 0)
	{
		//prevent duplicate commands from being repeatedly logged
		if(cmd != history.front())
		{
			//prevent the list from growing indefinitely
			if(history.size() > MAX_HISTORY)
				history.pop_back();

			history.push_front(cmd);
		}
	}
	else
	{
		history.push_front(cmd);
	}

	history_index = -1;
	cmd = "";
	command->set_text("");
}

void CLI_Command::backspace()
{
	//get the text directly from the screen, in case the user is modifying a past command
	cmd = command->get_text();

	if(cmd.length() > 0)
	{
		cmd.pop_back();
		command->set_text(cmd);
		mark_dirty();
	}
}

void CLI_Command::update_completion()
{

}
