
#include <iostream>
#include "utils.h"

void collector::print_SDL_error(std::string message)
{
	std::cout << message << std::endl << SDL_GetError() << std::endl;
}
