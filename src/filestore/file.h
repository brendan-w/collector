
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <SDL.h>

#include <thumbnail.h>


typedef std::vector<std::string> tag_vector;
typedef std::unordered_set<std::string> tag_set;


//SDL_Point =  8 bytes
//this = 4 bytes
typedef struct {
	uint16_t x;
	uint16_t y;
} File_Point;


class File
{
	public:
		File(std::string path);
		~File();

		tag_set get_tags();
		size_t get_size();

		std::string get_path();
		std::string get_full_path();
		void set_path(std::string new_path) { path = new_path; }

		void unload();
		Thumbnail* get_thumb();

		File_Point grid_pos;
		File_Point thumb_pos;

	private:
		std::string path;
		Thumbnail* thumb;
};

//file_vector
typedef std::vector<File*> file_vector;
typedef file_vector::iterator file_vector_it;

//file_set
typedef std::unordered_set<File*> file_set;
typedef file_set::iterator file_set_it;
