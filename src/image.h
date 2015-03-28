
#pragma once

#include <string>

#include <SDL.h>

#include <texture.h>
#include <text.h>



class Image: public Texture
{
	public:
		Image(std::string path);
		~Image();
};
