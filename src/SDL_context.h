
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <config.h>


class SDL_context
{
	public:
		SDL_context();
		~SDL_context();

		SDL_Window* get_window() { return window;}
		SDL_Renderer* get_renderer() { return renderer; }
		TTF_Font* get_font() { return font; }

		void submit(Uint32 type, void* data1=NULL, void* data2=NULL);

		void clear();
		void present();

		void set_color(Color c);
		void set_color(SDL_Color color);
		void fill_rect(SDL_Rect &rect);
		bool rect_in_window(SDL_Rect &rect);

		bool succeeded() { return success; }

	private:
		bool success = true;

		SDL_Window* window = NULL;
		SDL_Renderer* renderer = NULL;
		TTF_Font* font = NULL;
};
