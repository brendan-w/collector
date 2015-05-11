

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm> //sort()

#include <utils.h> //starts_with(), set_union
#include <filestore/file.h>
#include <filestore/selection.h>



static bool tag_entry_compare(Tag_Entry* A, Tag_Entry* B)
{
	return A->files.size() > B->files.size();
}



Selection::Selection(Selector* s, file_vector* all, file_set fs, entry_set es)
{
	selector = s;
	all_files = all;
	files = fs;

	//dump the set to a sortable vector
	entry_vector subtag_entries;
	for(Tag_Entry* entry: es)
	{
		//strain out lone subtags
		if(entry->files.size() > 1)
		subtag_entries.push_back(entry);
	}

	//sort in descending number of files per tag
	std::sort(subtag_entries.begin(),
			  subtag_entries.end(),
			  tag_entry_compare);
	
	tag_set selected;
	set_union(selected, selector->get_tag_intersections());
	set_union(selected, selector->get_tag_exclusions());

	//convert entries to plain-text tags
	for(Tag_Entry* entry: subtag_entries)
	{
		std::string tag = entry->tag;

		//strain out tags that were used to select this file set
		if(selected.find(tag) == selected.end())
		{
			subtag_set.insert(tag);
			subtags.push_back(tag);
		}
	}
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

bool Selection::has_subtag(std::string partial)
{
	for(std::string tag: subtags)
	{
		if(partial == tag)
			return true;
	}
	return false;
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
