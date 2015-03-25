
#pragma once

#include "file.h"

class Selection
{
	public:
		Selection();
		~Selection();
		void add_file(File* file);
		const file_set get_files() { return files; }
	private:
		file_set files;
};
