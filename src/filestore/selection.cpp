


#include <filestore/file.h>
#include <filestore/selection.h>


Selection::Selection(file_vector* all)
{
	all_files = all;
}

Selection::~Selection()
{

}

void Selection::add_file(File* file)
{
	if(files.find(file) == files.end())
		files.insert(file);
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
