
#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <SDL_utils.h>


SDL_Surface* CreateSurface(int w, int h)
{
	Uint32 rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	SDL_Surface* surface = SDL_CreateRGBSurface(
		0,
		w, h,
		32,
		rmask, gmask, bmask, amask
	);

	return surface;
}

void print_message(std::string message)
{
	std::cout << message << std::endl;
}

void print_SDL_error(std::string message)
{
	std::cout << message << std::endl << SDL_GetError() << std::endl;
}

void print_TTF_error(std::string message)
{
	std::cout << message << std::endl << TTF_GetError() << std::endl;
}

void print_IMG_error(std::string message)
{
	std::cout << message << std::endl << IMG_GetError() << std::endl;
}

bool point_in_rect(SDL_Point* p, SDL_Rect* r)
{
	return (p->x >= r->x) && (p->x <= (r->x + r->w)) &&
	       (p->y >= r->y) && (p->y <= (r->y + r->h));
}

bool colors_equal(SDL_Color & a, SDL_Color & b)
{
	return ((a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a));
}
