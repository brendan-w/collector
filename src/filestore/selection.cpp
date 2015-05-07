

#include <iterator>

#include <filestore/file.h>
#include <filestore/selection.h>


Selection::Selection(file_vector* all)
{
	all_files = all;
}

Selection::Selection(file_vector* all, file_set fs)
{
	all_files = all;
	files = fs;
}

Selection::~Selection()
{
	//if it was loaded, make sure to release it
	// for(File* file: files)
		// delete file->data;
}

bool Selection::has(File* file)
{
	return (files.find(file) != files.end());
}

File* Selection::at(size_t i)
{
	file_set_it it = begin();
	std::advance(it, i);
	return *it;
}

File* Selection::all_at(size_t i)
{
	if(i < all_size())
		return (*all_files)[i];
	else
		return NULL;
}
