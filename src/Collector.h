
#pragma once

#include <SDL.h>

namespace collector
{
	// main program window
	class Collector
	{
		public:
			int main();
		private:
			bool init();
			bool destruct();
			SDL_Window* window;
			SDL_Renderer* renderer;
	};
}
