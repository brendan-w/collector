
#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <SDL.h>

#include <texture.h>


typedef std::unordered_set<std::string> tag_set;


class File
{
	public:
		File(std::string path);
		~File();

		void load();
		void unload();

		tag_set get_tags();
		size_t get_size();

		std::string get_path();
		std::string get_full_path();
		void set_path(std::string new_path) { path = new_path; }

		SDL_Point point;
		Texture* texture;

	private:
		std::string path;
};

//file_vector
typedef std::vector<File*> file_vector;
typedef file_vector::iterator file_vector_it;

//file_set
typedef std::unordered_set<File*> file_set;
typedef file_set::iterator file_set_it;
