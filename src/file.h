
#pragma once

#include <string>
#include <unordered_set>
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

		std::unordered_set<std::string> get_tags();
		std::string get_file_path();
		void set_file_path(std::string new_path);
	private:
		bool find_next(size_t& start, size_t& len);

		std::string path;
		SDL_Rect rect;
		Texture* texture;
};
