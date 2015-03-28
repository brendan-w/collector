


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
	for(File* file: files)
		file->unload();
}

bool Selection::has(File* file)
{
	return (files.find(file) != files.end());
}

File* Selection::all_at(size_t i)
{
	if(i < all_size())
		return (*all_files)[i];
	else
		return NULL;
}
