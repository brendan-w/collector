
#pragma once


#include <string>
#include <vector>
#include <unordered_map>

#include <filestore/types.h>




class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		Selection* select(Selector* selector);

	private:
		file_vector files;
		tag_map tags;

		bool has_tag(const std::string & tag);
		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		tag_set tags_for_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
