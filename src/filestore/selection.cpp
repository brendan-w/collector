

#include <string>
#include <iostream>
#include <iterator>

#include <utils.h> //starts_with()
#include <filestore/file.h>
#include <filestore/selection.h>


Selection::Selection(Selector* s, file_vector* all, file_set fs, tag_vector st)
{
	selector = s;
	all_files = all;
	files = fs;
	subtags = st;
}

Selection::~Selection()
{
	delete selector;

	// for(File* file: files)
	// 	file->unload();
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

std::string Selection::auto_complete(std::string partial)
{
	for(std::string tag: subtags)
	{
		if(starts_with(tag, partial))
			return tag;
	}

	return partial;
}
