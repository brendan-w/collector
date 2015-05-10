

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm> //sort()
#include <unordered_map>

#include <utils.h> //starts_with()
#include <filestore/file.h>
#include <filestore/selection.h>


Selection::Selection(Selector* s, file_vector* all, file_set fs)
{
	selector = s;
	all_files = all;
	files = fs;

	// load_subtags();
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

/*
//sorts tags into descending frequency
static bool tag_freq_compare(tag_freq A, tag_freq B)
{
	return A.second > B.second;
}

void Selection::load_subtags()
{
	std::sort(subtags.begin(), subtags.end(), tag_freq_compare);
}
*/
