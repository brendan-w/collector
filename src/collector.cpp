

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <collector.h>
#include <config.h>
#include <event.h>
#include <filestore/file.h>
#include <filestore/filestore.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <display/display.h>


//forward declare
static bool init();
static void close();


//global contexts and resources
Config* config = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

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
						display->on_resize();
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

				case SDL_MOUSEWHEEL:
					display->on_wheel(e.wheel);
					break;

				default:
					//user events are registered at runtime, so can't be used in the switch
					if(e.type == SELECTOR)
					{
						//process the Selector, and broadcast the resulting Selection
						Selection* s = filestore->select((Selector*) e.user.data1);
						submit(SELECTION, (void*) s);
					}
					else if(e.type == SELECTION)
					{
						display->on_selection((Selection*) e.user.data1);
					}
					else if(e.type == FILE_INFO)
					{
						display->on_file_info((File*) e.user.data1);
					}
					break;
			}
		}

		//render changes
		setRenderDrawColor(renderer, config->get_color(BACKGROUND));
		SDL_RenderClear(renderer);

		display->render();

		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}

	close();

	return EXIT_SUCCESS;
}


static bool init()
{
	/*
		Config
	*/

	config = new Config;

	/*
		SDL2
	*/

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		print_SDL_error("SDL_Init Error");
		return false;
	}

	window = SDL_CreateWindow("Collector",
                              config->window.x,
                              config->window.y,
                              config->window.w,
                              config->window.h,
                              config->get_window_flags());
	if(window == NULL)
	{
		print_SDL_error("SDL_CreateWindow Error");
		return false;
	}

	renderer = SDL_CreateRenderer(window,
                                  -1,
                                  config->get_render_flags());
	if(renderer == NULL)
	{
		print_SDL_error("SDL_CreateRenderer Error");
		return false;
	}

	SDL_StartTextInput();

	if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		print_SDL_error("SDL_SetRenderDrawBlendMode Error");
		return false;
	}

	/*
		SDL2_ttf
	*/

	if(TTF_Init() != 0)
	{
		print_TTF_error("SDL_ttf could not initialize");
		return false;
	}

	font = TTF_OpenFont(config->font_path.c_str(),
						config->font_size);
	if(font == NULL)
	{
		print_TTF_error("Failed to load font: " + config->font_path);
		return false;
	}

	/*
		SDL2_image
	*/

	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

	if((IMG_Init(flags) & flags) != flags)
	{
		print_IMG_error("SDL_image could not initialize");
		return false;
	}

	/*
		Event Registration
	*/

	if(!init_events())
	{
		print_message("Failed to register custom events");
		return false;
	}

	/*
		Init the FileStore (performs the `find` scan)
	*/

	filestore = new FileStore;

	/*
		Create the display, with initial, empty, selection
	*/

	display = new Display(filestore->select(NULL));

	return true;
}

static void close()
{
	/*
		FileStore
	*/

	delete filestore;

	/*
		Display
	*/

	delete display;

	/*
		SDL2_image
	*/

	IMG_Quit();

	/*
		SDL2_ttf
	*/

	TTF_CloseFont(font);
	TTF_Quit();

	/*
		SDL2
	*/

	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	/*
		Config
	*/

	delete config;
}


/*
	General SDL utils
*/


void setRenderDrawColor(SDL_Renderer* r, SDL_Color color)
{
    SDL_SetRenderDrawColor(r,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
}

bool rectInWindow(SDL_Rect &rect)
{
	SDL_Rect screen = config->get_window_rect();
	return SDL_HasIntersection(&screen, &rect);
}

void print_message(std::string message)
{
	std::cout << message << std::endl;
}

void print_SDL_error(std::string message)
{
	std::cout << message << std::endl << SDL_GetError() << std::endl;
}

void print_TTF_error(std::string message)
{
	std::cout << message << std::endl << TTF_GetError() << std::endl;
}

void print_IMG_error(std::string message)
{
	std::cout << message << std::endl << IMG_GetError() << std::endl;
}
