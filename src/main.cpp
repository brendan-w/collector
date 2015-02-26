
#include <iostream>
#include <string.h>
#include "SDL.h"
#include "Collector.h"


int main(int argc, char * argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	collector::Collector app;

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

	SDL_Quit();

	return 0;
}
