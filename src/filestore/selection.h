
#pragma once

#include <filestore/file.h>


class Selection
{
	public:
		Selection(file_vector* all);
		Selection(file_vector* all, file_set fs);
		~Selection();

		bool has(File* file);

		file_set_it begin() { return files.begin(); }
		file_set_it end()   { return files.end(); }
		size_t size()       { return files.size(); }
		File* at(size_t i);

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
