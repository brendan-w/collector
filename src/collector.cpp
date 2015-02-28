

#include <SDL.h>
#include "collector.h"
#include "display.h"
#include "cli.h"
#include "utils.h"


//forward declare
bool start_SDL();
void stop_SDL();


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


int main(int argc, char * argv[])
{
	//only run if initialization was successful
	bool running = start_SDL();

	if(!running)
	{
		stop_SDL();
		return EXIT_FAILURE;
	}

	//create the two main screen elements
	Display display;
	CLI cli;

	//main event loop
	SDL_Event e;

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

		SDL_Delay(33);
	}

	stop_SDL();

	return EXIT_SUCCESS;
}


bool start_SDL()
{

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		print_SDL_error("SDL_Init Error");
		return false;
	}

	window = SDL_CreateWindow("Collector",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640,
                              480,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if(window == NULL)
	{
		print_SDL_error("SDL_CreateWindow Error");
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		print_SDL_error("SDL_CreateRenderer Error");
		SDL_Quit();
		return false;
	}

	//draw initial background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}


void stop_SDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
