
#pragma once

#include <string>

#include <SDL.h>

#include <texture.h>
#include <text.h>



class Thumbnail: public Texture
{
	public:
		Thumbnail(std::string path);
		~Thumbnail();
		bool loaded();
	private:
		bool success = false;
};
