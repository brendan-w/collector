
#pragma once

#include <string>
#include <SDL.h>
#include "texture.h"


class File
{
	public:
		File(std::string path);
		~File();

		void load();
		void unload();
		void render();

		std::string get_path();
		void set_path(std::string new_path);

		SDL_Rect rect;
	private:
		std::string path;
		Texture* texture;
};
