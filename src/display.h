
#pragma once

#include <SDL.h>

#include "cli.h"


class Display
{
	public:
		Display();
		~Display();
		void render();
		void on_resize();
		void on_key(SDL_KeyboardEvent &e);
		void on_text(SDL_TextInputEvent &e);

	private:
		CLI* cli;

		void send_quit();
		void send_selector();
};
