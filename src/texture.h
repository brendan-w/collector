
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
		void render(int _x, int _y);
		void render(int _x, int _y, int _w, int _h);
		int width();
		int height();
	private:
		SDL_Texture* texture;
		int w;
		int h;
};
