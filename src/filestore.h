
#pragma once


#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "file.h"


typedef std::vector<File*> file_vector;
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

		file_vector files;

	private:
		tag_map tags;

		void insert_file(File* file);
		tag_set get_tags(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
