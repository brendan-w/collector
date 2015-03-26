
#pragma once

#include <filestore/file.h>


class Selection
{
	public:
		Selection(file_vector* all);
		~Selection();

		void add_file(File* file);

		bool has(File* file);

		file_set_it begin() { return files.begin(); }
		file_set_it end()   { return files.end(); }
		size_t size()       { return files.size(); }

		file_vector_it all_begin() { return all_files->begin(); }
		file_vector_it all_end()   { return all_files->end(); }
		size_t all_size()          { return all_files->size(); }
		File* all_at(size_t i);

	private:
		 //set of Files selected by the previous Selector
		file_set files;
		
		//the whole file list
		file_vector* all_files;
};
