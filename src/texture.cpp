

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "texture.h"
#include "collector.h"


Texture::Texture()
{
	texture = NULL;
	w = 0;
	h = 0;
}


Texture::~Texture()
{
	free();
}


void Texture::free()
{
	if(texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		w = 0;
		h = 0;
	}
}


bool Texture::load_surface(SDL_Surface* surface)
{
	free();

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if(texture == NULL)
	{
		print_SDL_error("Failed to create texture from surface");
		return false;
	}

	w = surface->w;
	h = surface->h;

	return true;
}

/*
bool Texture::load_image(std::string filename)
{
	SDL_Surface* surface = IMG_Load(filename.c_str());

	if(surface == NULL)
	{
		print_IMG_error("Failed to create surface from image file");
		return false;
	}

	bool success = load_surface(surface);
	SDL_FreeSurface(surface);
	return success;
}
*/

void Texture::render(int x, int y)
{
	if((texture != NULL) && (w > 0) && (h > 0))
	{
		SDL_Rect dest = { x, y, w, h };
		SDL_RenderCopy(renderer, texture, NULL, &dest);
	}
}

int Texture::width()
{
	return w;
}

int Texture::height()
{
	return h;
}
