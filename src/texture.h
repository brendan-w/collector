
#pragma once

#include <string>

#include <SDL.h>

class Texture
{
	public:
		Texture();
		virtual ~Texture();
		void free();
		bool load_surface(SDL_Surface* surface);
		void render(int x, int y);
		int width();
		int height();
	private:
		SDL_Texture* texture;
		int w;
		int h;
};
