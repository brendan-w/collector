
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
	
	SDL_Delay(1000);
    SDL_Quit();

    return 0;
}
