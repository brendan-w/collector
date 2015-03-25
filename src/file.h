
#pragma once

#include <string>
#include <list>
#include <unordered_set>

#include <SDL.h>

#include "texture.h"


typedef std::unordered_set<std::string> tag_set;


class File
{
	public:
		File(std::string path);
		~File();

		void load();
		void unload();

		tag_set get_tags();

		std::string get_path() { return path; }
		void set_path(std::string new_path) { path = new_path; }

		SDL_Point point;

	private:
		std::string path;
		Texture* texture;
};

//file_list
typedef std::list<File*> file_list;
typedef file_list::iterator file_list_it;

//file_set
typedef std::unordered_set<File*> file_set;
typedef file_set::iterator file_set_it;
