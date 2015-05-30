
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# collector.cpp                                                        #
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
#include <string.h>
#include <stdio.h>

#include <SDL.h>

#include <collector.h>
#include <SDL_Context.h>
#include <config.h>
#include <filestore/types.h>
#include <filestore/filestore.h>
#include <display/display.h>
#include <utils.h>


#ifdef _WIN32
	#include <direct.h>
	#define getcwd _getcwd
#else
	#include <unistd.h> //getcwd()
#endif


//global contexts and resources
Config* config = NULL;
SDL_Context* sdl = NULL;

//the two main components
FileStore* filestore = NULL;
Display* display = NULL;


static void close()
{
	delete display;
	delete filestore;
	delete sdl;
	delete config;
}


static void help()
{
	printf("\
Usage:\n\
\tcollector [working path]\n\
\n\
By default, collector uses you current working directory as the working path\n\
By default, selections (symlinks) are exported to /tmp/collector\n\
\n\
Controls:\n\
\tTAB                  autocomplete tag\n\
\tCTRL + TAB           toggle between SELECT and COMMAND mode\n\
\tRETURN               in SELECT mode: exports the current selection to /tmp\n\
\t                     in COMMAND mode: executes the command (see below)\n\
\tDouble_Click         open file\n\
\tCTRL + Left_Click    include file\n\
\tCTRL + Right_Click   exclude file\n\
\tCTRL + a             included all files in current selection\n\
\tCTRL + i             clear included files\n\
\tCTRL + e             clear excluded files\n\
\tESCAPE               quit collector\n\
\n\
Commands:\n\
\t+<tag>               adds <tag> to the current selection\n\
\t-<tag>               removes <tag> from the current selection\n\
\n\
For issues and documentation: https://github.com/brendanwhitfield/collector\n");
}


int main(int argc, char * argv[])
{
	//handle cli input and pick the working directory
	std::string cwd = "";

	if(argc == 1)
	{
		cwd = std::string(getcwd(NULL, 0));
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "--help") == 0)
		{
			help();
			return EXIT_SUCCESS;
		}
		else
		{
			if(dir_exists(argv[1]))
			{
				cwd = std::string(argv[1]);
			}
			else
			{
				printf("Error:\n\tInvalid directory: %s\n\n", argv[1]);		
				return EXIT_FAILURE;	
			}
		}
	}
	else
	{
		help();
		return EXIT_FAILURE;
	}

	/*
		init
	*/

	config    = new Config(cwd);
	sdl       = new SDL_Context;
	filestore = new FileStore;
	display   = new Display(filestore->select(new Selector())); //initial, empty, selection

	if(!sdl->succeeded())
	{
		close();
		return EXIT_FAILURE;
	}

	/*
		start the main loop
	*/

	SDL_Event e;
	bool running = true;

	while(running)
	{
		SDL_WaitEvent(NULL);

		//event pump
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				//dispatch SDL events to their respective handlers
				case SDL_QUIT:            running = false; break;
				case SDL_WINDOWEVENT:     display->on_window_event(e.window); break;
				case SDL_KEYDOWN:         display->on_key_down(e.key); break;
				case SDL_KEYUP:           display->on_key_up(e.key); break;
				case SDL_TEXTINPUT:       display->on_text(e.text); break;
				case SDL_MOUSEMOTION:     display->on_motion(e.motion); break;
				case SDL_MOUSEBUTTONDOWN: display->on_click(e.button); break;
				case SDL_MOUSEWHEEL:      display->on_wheel(e.wheel); break;

				default:
					//user events are registered at runtime, so can't be used in the switch
					if(e.type == SELECTOR)
						display->on_selection(filestore->select((Selector*) e.user.data1));
					else if(e.type == OPERATION)
						filestore->operation((Operation*) e.user.data1);
					else if(e.type == RENDER_THUMBS)
						display->request_render(RENDER_THUMBS);
					else if(e.type == STATE_CHANGE)
						display->on_state_change();
					break;
			}
		}

		//render changes
		display->render();
		sdl->present();
		SDL_Delay(33);
	}

	close();
	return EXIT_SUCCESS;
}

