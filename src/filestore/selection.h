
#pragma once

#include <filestore/file.h>


class Selection
{
	public:
		Selection(file_list_it b, file_list_it e, size_t s);
		~Selection();

		void add_file(File* file);

		bool has(File* file);

		file_set_it begin() { return files.begin(); }
		file_set_it end() { return files.end(); }
		size_t size() { return files.size(); }

		file_list_it all_begin() { return _all_begin; }
		file_list_it all_end() { return _all_end; }
		size_t all_size() { return _all_size; }

	private:
		 //set of Files selected by the previous Selector
		file_set files;
		
		//iterators for the whole file list
		file_list_it _all_begin;
		file_list_it _all_end;

		//total number of files in the FileStore
		size_t _all_size = 0;
};
