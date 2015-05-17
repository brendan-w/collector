
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
		void operation(Selection* selection, Operation* operation);

	private:
		file_vector files;
		tag_map tags;

		bool has_tag(const std::string & tag);
		void insert_file(File* file);
		tag_set tags_for_file(File* file);

		void add_tag_on_file(File* file, const std::string & tag);
		void remove_tag_on_file(File* file, const std::string & tag);
};
