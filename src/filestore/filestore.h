
#pragma once


#include <string>

#include <filestore/types.h>


class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		Tag_Info* tag_info(Tag_Info* c);
		Selection* select(Selector* selector);

	private:
		file_vector files;
		tag_map tags;

		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
