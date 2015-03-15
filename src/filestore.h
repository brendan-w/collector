
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

	private:
		std::string root;
		file_vector files;
		tag_map tags;

		void insert_file(File* file);
		tag_set get_tags(File* file);
		unsigned int levenshtein_distance(const std::string &s1, const std::string &s2);
};
