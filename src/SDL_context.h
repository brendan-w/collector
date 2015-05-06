
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


class SDL_context
{
	public:
		SDL_context();
		~SDL_context();

		SDL_Window* get_window() { return window;}
		SDL_Renderer* get_renderer() { return renderer; }
		TTF_Font* get_font() { return font; }

		void clear();
		void present();

		void setColor(SDL_Color color);
		void fillRect(SDL_Rect &rect);
		bool rectInWindow(SDL_Rect &rect);

		bool succeeded() { return success; }

	private:
		bool success = true;

		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		TTF_Font* font = NULL;
};
