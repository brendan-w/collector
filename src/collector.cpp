

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "collector.h"
#include "config.h"
#include "display.h"
#include "cli.h"


//forward declare
bool init();
void close();


//global contexts and resources
Config* config = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;


int main(int argc, char * argv[])
{
	if(!init())
	{
		close();
		return EXIT_FAILURE;
	}

	//create the two main screen elements
	Display display;
	CLI cli;

	//main event loop
	SDL_Event e;
	bool running = true;

	while(running)
	{
		//event pump
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
				case SDL_MOUSEBUTTONDOWN:
					running = false;
					break;

				// cli.cpp handles all keyboard input
				// and creates new events when needed
				case SDL_KEYDOWN:
					cli.handle_key(e.key);
					break;

				case SDL_TEXTINPUT:
					cli.handle_text(e.text);
					break;

				case SDL_WINDOWEVENT:
					if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						cli.set_dirty();
					}
					break;

				case SDL_USEREVENT:
					break;

				default:
					break;
			}
		}

		//render changes
		setRenderDrawColor(renderer, config->get_background_color());
		SDL_RenderClear(renderer);

		cli.render();

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
                              config->get_window_x(),
                              config->get_window_y(),
                              config->get_window_w(),
                              config->get_window_h(),
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

	/*
		SDL2_ttf
	*/

	//Initialize SDL_ttf
	if(TTF_Init() != 0)
	{
		print_TTF_error("SDL_ttf could not initialize");
		return false;
	}

	font = TTF_OpenFont(config->get_font_path().c_str(),
						config->get_font_size());
	if(font == NULL)
	{
		print_TTF_error("Failed to load font: " + config->get_font_path());
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

	return true;
}

void close()
{
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
	General program-wide utils
*/


void setRenderDrawColor(SDL_Renderer* r, SDL_Color color)
{
    SDL_SetRenderDrawColor(r,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
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
