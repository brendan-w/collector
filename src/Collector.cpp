

#include <SDL.h>
#include "Collector.h"
#include "utils.h"

using namespace collector;


int Collector::main()
{
	//main event loop
	SDL_Event e;

	//only run if initialization was successful
	bool running = init();

	while(running)
	{
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
	}

	destruct();

	return EXIT_SUCCESS;
}

bool Collector::init()
{

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		print_SDL_error("SDL_Init Error");
		return false;
	}

	window = SDL_CreateWindow("Hello World!",
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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		print_SDL_error("SDL_CreateRenderer Error");
		SDL_Quit();
		return false;
	}

	std::string imagePath = "../hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	if(bmp == NULL)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		print_SDL_error("SDL_LoadBMP Error");
		SDL_Quit();
		return false;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if(tex == NULL)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		print_SDL_error("SDL_CreateTextureFromSurface Error");
		SDL_Quit();
		return false;
	}

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(tex);
}

bool Collector::destruct()
{

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return true;
}
