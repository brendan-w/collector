
#pragma once

#include <iostream>
#include <string.h>
#include "SDL.h"


namespace collector
{
	// main program window
	class Collector
	{
		public:
			Collector();
			virtual ~Collector();
		private:
			SDL_Window* window;
			SDL_Renderer* renderer;
	};
}
