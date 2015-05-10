
#pragma once

#include <utility>

#include <filestore/file.h>
#include <filestore/selector.h>


typedef std::vector<std::string> str_tag_vector;


class Selection
{
	public:
		Selection(Selector* s, file_vector* all, file_set fs);
		~Selection();

		bool has(File* file);
		str_tag_vector get_subtags() { return subtags; }

		//the selection
		const file_set_it begin() { return files.begin(); }
		const file_set_it end()   { return files.end(); }
		size_t size()       { return files.size(); }
		File* at(size_t i);

		//all files
		const file_vector_it all_begin() { return all_files->begin(); }
		const file_vector_it all_end()   { return all_files->end(); }
		size_t all_size()          { return all_files->size(); }
		File* all_at(size_t i);

		std::string auto_complete(std::string partial);

	private:
		 //set of Files selected by the previous Selector
		file_set files;
		
		//the whole file list
		file_vector* all_files;

		//the selector that generated this selection
		Selector* selector;

		//subtags
		str_tag_vector subtags;

		//functions
		// void load_subtags();
};
