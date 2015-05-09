

#include <string>
#include <iostream>
#include <iterator>
#include <unordered_map>

#include <filestore/file.h>
#include <filestore/selection.h>


typedef std::unordered_map<std::string, int> tag_freq;


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

void Selection::load_subtags()
{
	tag_freq freqs;
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

	for(auto result: freqs)
	{
		std::cout << result.first << " - " << result.second << std::endl;
	}
}
