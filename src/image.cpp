

#include <SDL_image.h>

#include <collector.h> //print_IMG_error()
#include <utils.h> //file_exists()
#include <image.h>


Image::Image(std::string path)
{
	if(!file_exists(path.c_str()))
	{
		print_message("File doesn't exist: " + path);
		return;
	}

	SDL_Surface* surface = IMG_Load(path.c_str());

	if(surface == NULL)
	{
		print_IMG_error("Failed to create surface from image file");
		return;
	}

	load_surface(surface);
	SDL_FreeSurface(surface);
}

Image::~Image()
{

}
