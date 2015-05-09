
#pragma once


#include <string>

#include <filestore/file.h>
#include <filestore/selector.h>
#include <filestore/selection.h>


class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		tag_set auto_complete(const std::string &partial_tag);
		Selection* select(Selector* selector);

	private:
		file_vector files;
		tag_map tags;

		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
