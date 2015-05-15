

#include <SDL.h>

#include <collector.h>
#include <SDL_Context.h>
#include <config.h>
#include <filestore/types.h>
#include <filestore/filestore.h>
#include <display/display.h>


//forward declare
static bool init();
static void close();


//global contexts and resources
Config* config = NULL;
SDL_Context* sdl = NULL;

//the two main components
FileStore* filestore = NULL;
Display* display = NULL;


int main(int argc, char * argv[])
{
	if(!init())
	{
		close();
		return EXIT_FAILURE;
	}

	//main event loop
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
				case SDL_QUIT:
					running = false;
					break;

				case SDL_WINDOWEVENT:
					if(e.window.event == SDL_WINDOWEVENT_RESIZED)
						display->resize();
					break;

				case SDL_KEYDOWN:
					display->on_key(e.key);
					break;

				case SDL_TEXTINPUT:
					display->on_text(e.text);
					break;

				case SDL_MOUSEMOTION:
					display->on_motion(e.motion);
					break;

				case SDL_MOUSEBUTTONDOWN:
					display->on_click(e.button);
					break;

				case SDL_MOUSEWHEEL:
					display->on_wheel(e.wheel);
					break;

				default:
					//user events are registered at runtime, so can't be used in the switch
					if(e.type == SELECTOR)
					{
						//process the Selector, and broadcast the resulting Selection
						Selection* s = filestore->select((Selector*) e.user.data1);
						sdl->submit(SELECTION, (void*) s);
					}
					else if(e.type == SELECTION)
					{
						display->on_selection((Selection*) e.user.data1);
					}
					else if(e.type == FILE_INFO)
					{
						display->on_file_info((File*) e.user.data1);
					}
					else if(e.type == RENDER_THUMBS)
					{
						display->request_render(RENDER_THUMBS);
					}
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


static bool init()
{
	config    = new Config;
	sdl       = new SDL_Context;
	filestore = new FileStore;
	display   = new Display(filestore->select(new Selector())); //initial, empty, selection

	return sdl->succeeded();
}

static void close()
{
	delete display;
	delete filestore;
	delete sdl;
	delete config;
}
