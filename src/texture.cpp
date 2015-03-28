

#include <string>

#include <SDL.h>

#include <collector.h>
#include <texture.h>


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

void Texture::render(int _x, int _y)
{
	render(_x, _y, w, h);
}

void Texture::render(SDL_Rect *r)
{
	render(r->x, r->y, r->w, r->h);
}

void Texture::render(int _x, int _y, int _w, int _h)
{
	if((texture != NULL) && (w > 0) && (h > 0))
	{
		SDL_Rect dest = { _x, _y, _w, _h };
		if(rectInWindow(dest))
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
