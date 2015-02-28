

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "texture.h"
#include "collector.h"


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


bool Texture::load_surface(SDL_Surface* surface)
{
	free();

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if(texture == NULL)
	{
		print_SDL_error("Failed to create texture from surface");
		return false;
	}

	width = surface->w;
	height = surface->h;

	return true;
}


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


bool Texture::load_text(std::string text, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	if(surface == NULL)
	{
		print_TTF_error("Failed to render text to surface");
		return false;
	}

	bool success = load_surface(surface);
	SDL_FreeSurface(surface);
	return success;
}


void Texture::render(int x, int y)
{
	if(texture == NULL)
	{
		print_message("Can't render a NULL texture");
		return;
	}

	SDL_Rect dest = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}
