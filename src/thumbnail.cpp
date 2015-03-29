
#include <pthread.h>
#include <iostream>
#include <string>
// #include <algorithm>

#include <SDL_image.h>

#include <collector.h> //print_IMG_error()
#include <SDL_utils.h>
#include <config.h>
#include <utils.h> //file_exists()
#include <thumbnail.h>




//threaded image loader
static void* load(void* data)
{
	std::string* path = (std::string*) data;

	SDL_Surface* surface = IMG_Load(path->c_str());

	delete path;

	if(surface == NULL)
	{
		print_IMG_error("Failed to create surface from image file");
		return NULL;
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
		print_SDL_error("Failed to create surface");
		SDL_FreeSurface(surface);
		return NULL;
	}

	if(SDL_BlitScaled(surface, &src, thumb, &dest) != 0)
	{
		print_SDL_error("Failed to SDL_BlitScaled()");
		SDL_FreeSurface(surface);
		return NULL;
	}

	SDL_FreeSurface(surface);

	return (void*) thumb;
}





Thumbnail::Thumbnail(std::string p)
{
	std::string* path = new std::string(p);
	thread_running = true;
	texture = NULL;

	if(pthread_create(&thread, NULL, load, (void*) path))
	{
		thread_running = false;
		print_message("Failed to create thumbnail loading thread");
	}
}


Thumbnail::~Thumbnail()
{
	join(true);

	if(texture != NULL)
		delete texture;
}


void Thumbnail::join(bool force)
{
	if(thread_running)
	{
		SDL_Surface* thumb = NULL;

		if(force)
		{
			pthread_join(thread, (void**) &thumb);
		}
		else
		{
			if(pthread_tryjoin_np(thread, (void**) &thumb) != 0)
				return;
		}

		//load the surface into a texture
		thread_running = false;

		//if the load succeeded
		if(thumb != NULL)
		{
			texture = new Texture();
			if(!texture->load_surface(thumb))
			{
				delete texture;
				texture = NULL;
			}

			SDL_FreeSurface(thumb);
		}
	}
}


void Thumbnail::render(SDL_Rect* rect)
{
	join(false);

	if(texture == NULL)
		SDL_RenderFillRect(renderer, rect);
	else
		texture->render(rect);
}
