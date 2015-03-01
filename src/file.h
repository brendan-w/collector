
#pragma once

#include <string>
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
		Texture* texture;
};
