
#pragma once


#include <string>
#include <unordered_set>
#include <unordered_map>
#include "file.h"
#include "selector.h"
#include "selection.h"


typedef std::unordered_set<std::string> tag_set;
typedef std::unordered_map<std::string, file_set> tag_map;


class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		tag_set auto_complete(const std::string &partial_tag);
		Selection* select(Selector* selector);

		//getters/setters
		file_list_it begin() { return files.begin(); };
		file_list_it end()   { return files.end(); };

	private:
		file_list files;
		tag_map tags;

		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		tag_set tags_for_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
