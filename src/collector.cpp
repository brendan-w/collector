

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "collector.h"
#include "filestore.h"
#include "selector.h"
#include "selection.h"
#include "config.h"
#include "display.h"


//forward declare
bool init();
void close();


//global contexts and resources
Config* config = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

//the two main components
FileStore* filestore = NULL;
Display* display = NULL;

//SDL userevent types
Uint32 SELECTOR;


int main(int argc, char * argv[])
{
	if(!init())
	{
		close();
		return EXIT_FAILURE;
	}

	//trigger the initial screen cleanup
	display->on_resize(filestore->begin(),
					   filestore->end());

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
						display->on_resize(filestore->begin(),
										   filestore->end());
					break;

				case SDL_KEYDOWN:
					display->on_key(e.key);
					break;

				case SDL_TEXTINPUT:
					display->on_text(e.text);
					break;

				case SDL_MOUSEBUTTONDOWN:
					break;

				case SDL_MOUSEWHEEL:
					display->on_wheel(e.wheel);
					break;

				case SDL_USEREVENT:
					if(e.user.type == SELECTOR)
					{
						Selector* selector = (Selector*) e.user.data1;
						Selection* selection = filestore->select(selector);
						display->on_select(selection);
					}
					break;

				default:
					break;
			}
		}

		//render changes
		setRenderDrawColor(renderer, config->get_color(BACKGROUND));
		SDL_RenderClear(renderer);

		display->render(filestore->begin(),
						filestore->end());

		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}

	close();

	return EXIT_SUCCESS;
}


bool init()
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
                              config->window_x,
                              config->window_y,
                              config->window_w,
                              config->window_h,
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
		Register Custom User Events
	*/

	SELECTOR = SDL_RegisterEvents(1);
	if(SELECTOR == ((Uint32) -1 ))
	{
		print_message("Failed to register custom events");
		return false;
	}

	/*
		Create the display
	*/

	display = new Display;

	/*
		Init the FileStore (performs the `find` scan)
	*/

	filestore = new FileStore;

	return true;
}

void close()
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
	SDL_Rect screen = {
		0,
		0,
		config->window_w,
		config->window_h
	};
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
