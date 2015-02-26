
#include "Collector.h"
#include <iostream>
#include <stdlib.h>

using namespace collector;


Collector::Collector()
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if(window == NULL)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL)
	{
		SDL_DestroyWindow(window);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	std::string imagePath = "../hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	if(bmp == NULL)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if(tex == NULL)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return;
	}

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, tex, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(tex);
}


int Collector::main()
{
	//main event loop
	SDL_Event e;
	
	bool running = true;

	while(running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				running = false;
		}
		
		//render changes
	}

	return EXIT_SUCCESS;
}


Collector::~Collector()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
