
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
			bool start_SDL();
			void stop_SDL();
			SDL_Window* window;
			SDL_Renderer* renderer;
	};
}
