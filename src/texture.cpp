

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "texture.h"
#include "collector.h"
#include "utils.h"


Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;
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
		width = 0;
		height = 0;
	}
}

bool Texture::load_image(std::string filename)
{
	free();
	// SDL_Surface* loadedSurface = IMG_Load(filename.c_str());

	return true;
}

bool Texture::load_text(std::string text, SDL_Color color)
{
	free();

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	if(surface == NULL)
	{
		print_TTF_error("Failed to render text to surface");
		return false;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if(texture == NULL)
	{
		print_SDL_error("Failed to create texture from text surface");
		SDL_FreeSurface(surface);
		return false;
	}

	width = surface->w;
	height = surface->h;
	SDL_FreeSurface(surface);

	return true;
}

void Texture::render(int x, int y)
{
	if(texture == NULL)
	{
		print_message("Can't render a NULL texture");
		return;
	}

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}
