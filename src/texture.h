
#pragma once

#include <SDL.h>
#include <string>

class Texture
{
	public:
		Texture();
		~Texture();
		void free();
		bool load_image(std::string filename);
		bool load_text(std::string text, SDL_Color color);
		void render(int x, int y);
	private:
		SDL_Texture* texture;
		int width;
		int height;
};
