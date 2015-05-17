

#include <iostream>
#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/types.h>
#include <display/state.h>
#include <display/cli_command.h>




CLI_Command::CLI_Command(State* s) : DisplayObject(s)
{
	command    = new Text("", config->get_color(CLI_LIGHT));
	completion = new Text("", config->get_color(CLI_DARK));
}


CLI_Command::~CLI_Command()
{
	delete command;
	delete completion;
}


bool CLI_Command::on_key(SDL_KeyboardEvent &e)
{
	switch(e.keysym.sym)
	{
		case SDLK_RETURN:
			execute();
			break;
		case SDLK_BACKSPACE:
			backspace();
			break;
		case SDLK_TAB:
			//auto_complete();
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
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

	return false;
}


void CLI_Command::on_text(SDL_TextInputEvent &e)
{
	command->set_text(command->get_text() + e.text);
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
	std::string cmd = command->get_text();

	if(cmd.length() > 1)
	{
		switch(cmd.at(0))
		{
			case '+': //add tag to selection
				cmd = cmd.substr(1);
				sdl->submit(OPERATION,
							selection(),
							new Operation(cmd, ADD_TAG));

				command->set_text("");
				break;
			case '-': //remove tag from selection
				cmd = cmd.substr(1);
				sdl->submit(OPERATION,
							selection(),
							new Operation(cmd, REMOVE_TAG));

				command->set_text("");
				break;
			default:
				break;
		}
	}

	mark_dirty();
}

void CLI_Command::backspace()
{
	std::string s = command->get_text();
	if(s.length() > 0)
	{
		s.pop_back();
		command->set_text(s);
		mark_dirty();
	}
}

void CLI_Command::update_completion()
{

}
