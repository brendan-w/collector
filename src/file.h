
#pragma once

#include <string>
#include <set>
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

		std::set<std::string> get_tags();
		std::string get_file_path();
		void set_file_path(std::string new_path);
	private:
		std::string path;
		SDL_Rect rect;
		Texture* texture;
};
