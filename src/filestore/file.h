
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


//forward declare
class Tag_Entry;

typedef std::vector<Tag_Entry*> entry_vector;
typedef std::unordered_set<Tag_Entry*> entry_set;
typedef std::unordered_map<std::string, Tag_Entry*> tag_map;


class File
{
	public:
		File(std::string path);
		~File();

		//functions

		//opens the file in its default application
		//the filename is optional in case the symlink needs to be opened
		void open(std::string filepath);

		size_t get_size();
		std::string get_path();
		std::string get_full_path();
		std::string get_link_path();
		void set_path(std::string new_path);

		void unload();
		Thumbnail* get_thumb();

		//properties

		File_Point grid_pos;
		File_Point thumb_pos;

		entry_set tags;

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


class Tag_Entry
{
	public:
		std::string tag;
		file_set files;
};
