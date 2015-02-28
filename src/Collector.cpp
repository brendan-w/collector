

#include <iostream>
#include <SDL.h>
#include "Collector.h"
#include "utils.h"

using namespace collector;


int Collector::main()
{
	//main event loop
	SDL_Event e;

	//only run if initialization was successful
	bool running = start_SDL();

	while(running)
	{
		//pump events
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				running = false;
			else if(e.type == SDL_MOUSEBUTTONDOWN)
				running = false;
			else if(e.type == SDL_KEYDOWN)
				running = false;
		}

		//render changes

		SDL_Delay(33);
	}

	stop_SDL();

	return EXIT_SUCCESS;
}

bool Collector::start_SDL()
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

void Collector::stop_SDL()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
