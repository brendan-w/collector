
#pragma once

#include "file.h"

class Selection
{
	public:
		Selection();
		~Selection();
		void add_file(File* file);
		const file_set get_files() { return files; }
		size_t get_size() { return files.size(); }

		size_t total = 0; //total number of files in the FileStore
	private:
		file_set files; //set of Files selected by the previous Selector
};
