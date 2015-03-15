
#pragma once


#include <string>
#include <unordered_set>
#include <unordered_map>
#include "file.h"


typedef std::unordered_set<std::string> tag_set;
typedef std::unordered_set<File*> file_set;
typedef std::unordered_map<std::string, file_set> tag_map;


class FileStore
{
	public:
		FileStore();
		~FileStore();
	private:
		std::string root;
		file_set files;
		tag_map tags;
		tag_set get_tags(File* file);
		void exec_find();
};
