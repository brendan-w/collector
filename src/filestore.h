
#pragma once


#include <string>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include "file.h"


typedef std::list<File*> file_list;
typedef std::unordered_set<std::string> tag_set;
typedef std::unordered_set<File*> file_set;
typedef std::unordered_map<std::string, file_set> tag_map;


class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		tag_set auto_complete(const std::string &partial_tag);
		file_set query(const std::string &tag);

		file_list files;

	private:
		tag_map tags;

		void insert_file(File* file);
		tag_set get_tags(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
