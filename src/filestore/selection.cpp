


#include <filestore/file.h>
#include <filestore/selection.h>


Selection::Selection(file_list_it b, file_list_it e, size_t s)
{
	_all_begin = b;
	_all_end = e;
	_all_size = s;
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
