
#pragma once


#include <string>
#include <unordered_set>
#include <unordered_map>
#include "file.h"
#include "selector.h"


typedef std::unordered_set<std::string> tag_set;
typedef std::unordered_map<std::string, file_set> tag_map;


class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		tag_set auto_complete(const std::string &partial_tag);
		file_set* select(Selector* selector);

		//getters/setters
		const file_list get_files() { return files; };

	private:
		file_list files;
		tag_map tags;

		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		tag_set tags_for_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
