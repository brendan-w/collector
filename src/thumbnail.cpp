
#include <algorithm>

#include <SDL_image.h>

#include <collector.h> //print_IMG_error()
#include <SDL_utils.h>
#include <config.h>
#include <utils.h> //file_exists()
#include <thumbnail.h>


Thumbnail::Thumbnail(std::string path)
{
	success = false;

	if(!file_exists(path.c_str()))
	{
		// print_message("File doesn't exist: " + path);
		// print_message(path);
		return;
	}

	SDL_Surface* surface = IMG_Load(path.c_str());

	if(surface == NULL)
	{
		// print_IMG_error("Failed to create surface from image file");
		// print_message(path);
		return;
	}

	/*
		scale it to a thumbnail, preserve aspect, but make it square
	*/

	int x = 0;
	int y = 0;
	int src_size = 0;

	if(surface->w > surface->h)
	{
		src_size = surface->h;
		x = (surface->w - src_size) / 2;
	}
	else if(surface->h > surface->w)
	{
		src_size = surface->w;
		y = (surface->h - src_size) / 2;
	}

	SDL_Rect src = { x, y, src_size, src_size };
	SDL_Rect dest = { 0, 0, FILE_THUMB_SIZE, FILE_THUMB_SIZE };

	SDL_Surface* thumb = CreateSurface(FILE_THUMB_SIZE,
	                                   FILE_THUMB_SIZE);

	if(thumb == NULL)
	{
		// print_SDL_error("Failed to create surface");
		SDL_FreeSurface(surface);
		return;
	}

	if(SDL_BlitScaled(surface, &src, thumb, &dest) != 0)
	{
		// print_SDL_error("Failed to SDL_BlitScaled()");
		SDL_FreeSurface(surface);
		return;
	}

	load_surface(thumb);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(thumb);

	success = true;
}

Thumbnail::~Thumbnail()
{

}

bool Thumbnail::loaded()
{
	return success;
}
