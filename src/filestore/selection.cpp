

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

	load_subtags();
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
	for(tag_freq tag: subtags)
	{
		if(starts_with(tag.first, partial))
			return tag.first;
	}

	return partial;
}

//sorts tags into descending frequency
static bool tag_freq_compare(tag_freq A, tag_freq B)
{
	return A.second > B.second;
}

void Selection::load_subtags()
{
	tag_freq_set freqs;
	for(File* file: files)
	{
		tag_set file_tags = file->get_tags();
		for(std::string tag: file_tags)
		{
			//avoid listing subtags the user already requested
			if(!selector->has(tag))
			{
				//increment the frequency of this tag
				if(freqs.find(tag) == freqs.end())
					freqs[tag] = 1;
				else
					freqs[tag]++;
			}
		}
	}

	for(tag_freq tag: freqs)
	{
		if(tag.second > 1) //skip tags that only occur once
			subtags.push_back(tag);
	}

	std::sort(subtags.begin(), subtags.end(), tag_freq_compare);
}
