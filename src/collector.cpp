

#include <SDL.h>
#include <SDL_ttf.h>
#include "collector.h"
#include "config.h"
#include "display.h"
#include "cli.h"
#include "utils.h"


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
		//pump events
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
				case SDL_MOUSEBUTTONDOWN:
					running = false;
					break;
				case SDL_KEYDOWN:
					cli.handle_key(e.key);
					break;
				case SDL_TEXTINPUT:
					cli.handle_text(e.text);
					break;
				case SDL_USEREVENT:
					break;
				default:
					break;
			}
		}

		//render changes
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	
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
		load the config
	*/

	config = new Config;

	/*
		load SDL components
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

	//Initialize SDL_ttf
	if(TTF_Init() != 0)
	{
		print_TTF_error("SDL_ttf could not initialize! SDL_ttf Error");
		return false;
	}

	font = TTF_OpenFont(config->get_font_path().c_str(),
						config->get_font_size());
	if(font == NULL)
	{
		print_TTF_error("Failed to load font: " + config->get_font_path());
		return false;
	}

	return true;
}

void close()
{
	TTF_CloseFont(font);
	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_Quit();
	SDL_Quit();

	delete config;
}
