
#include <iostream>
#include <string>
#include <stdio.h>
#include <algorithm> //sort()

#include <collector.h>
#include <utils.h>
#include <filestore/types.h>
#include <filestore/filestore.h>



FileStore::FileStore()
{
	//run command to return /n delimited list of files in the current directory
	FILE* pipe = popen(config->find_cmd.c_str(), "r");

	if(!pipe)
		return;

	char *line = NULL;
	size_t size = 0;
	size_t root_path_length = config->cwd_path.length() + 1;

	while(!feof(pipe))
	{
		if(getline(&line, &size, pipe) != -1)
		{
			std::string path = std::string(line);
			
			//pop the ending newline
			path.pop_back();

			//remove root working directory
			path = path.substr(root_path_length, std::string::npos);

			//create a new File object, and save it in the vector
			insert_file(new File(path));
		}
	}

	pclose(pipe);
}


FileStore::~FileStore()
{
	for(File* file: files)
	{
		delete file;
	}

	files.clear();
	tags.clear();
}


Tag_Info* FileStore::tag_info(Tag_Info* c)
{
	c->set_completed(fuzzy_match(c->get_partial()));
	return c;
}


static bool tag_entry_compare(Tag_Entry* A, Tag_Entry* B)
{
	return A->files.size() > B->files.size();
}


//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	//result holders
	file_set r_files;
	entry_set selected_entries;

	if(selector != NULL)
	{
		tag_set intersections = selector->get_tag_intersections();

		bool first = true;
		for(std::string tag: intersections)
		{
			//prevent unknown tags from destroying the query
			if(has_tag(tag))
			{
				Tag_Entry* entry = tags[tag];
				selected_entries.insert(entry);

				if(first)
				{
					//the first tag to be processed is a subset of the universe
					r_files = entry->files;
					first = false;
				}
				else
				{
					file_set r_files_copy = r_files;
					file_set current_files = entry->files;

					set_intersect<file_set>(r_files,
											r_files_copy,
											current_files);
				}
			}
		}
	}

	//compute the set of subtags by performing a union
	entry_set r_subtags;
	for(File* file: r_files)
	{
		set_union(r_subtags, file->tags);
	}

	//dump the set to a sortable vector
	entry_vector subtag_entries;
	for(Tag_Entry* entry: r_subtags)
	{
		//strain out lone subtags
		if(entry->files.size() == 1)
			continue;

		//strain out tags that were used to select this file set
		if(selected_entries.find(entry) != selected_entries.end())
			continue;	

		subtag_entries.push_back(entry);
	}

	//sort in descending number of files per tag
	std::sort(subtag_entries.begin(),
			  subtag_entries.end(),
			  tag_entry_compare);
	
	//convert entries to plain-text tags
	tag_vector subtags;
	for(Tag_Entry* entry: subtag_entries)
	{
		subtags.push_back(entry->tag);
	}

	//done selecting
	return new Selection(selector, &files, r_files, subtags);
}


bool FileStore::has_tag(const std::string & tag)
{
	return (tags.find(tag) != tags.end());
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	tag_set file_tags = tags_for_file(file);


	//first iteration, populate the tag map with any new tags
	for(std::string tag: file_tags)
	{
		Tag_Entry* entry = NULL;

		if(!has_tag(tag))
		{
			//create a new entry object for this tag
			entry = new Tag_Entry;
			entry->tag = tag;
			entry->files.insert(file);

			tags[tag] = entry;
		}
		else
		{
			//add the file to the correct tag file_set
			entry = tags[tag];
			entry->files.insert(file);
		}

		//give the file a pointer to each of its Tag_Entry
		file->tags.insert(entry);
	}
}


tag_set FileStore::tags_for_file(File* file)
{
	tag_set tags;

	//copy the path, so to_lower won't affect the original
	std::string p = file->get_path();
	to_lower(p);

	size_t prev = 0;
	size_t pos = 0;

	//while there is another delimeter
	while((pos = p.find_first_of(config->tag_delim, prev)) != std::string::npos)
	{
		if(pos > prev)
		{
			tags.insert(p.substr(prev, pos-prev));
		}
		prev = pos + 1;
	}

	//add the last tag to the set
	if(prev < p.length())
	{
		tags.insert(p.substr(prev, std::string::npos));
	}

	return tags;
}


//return the tag with the nearest edit distance
std::string FileStore::fuzzy_match(const std::string & partial_tag)
{
	std::string nearest_str = "";
	size_t nearest = 0;

	//prepopulate the initial result
	auto it = tags.begin();
	nearest_str = it->first;
	nearest = levenshtein_distance(partial_tag, nearest_str);
	++it;

	for( ; it != tags.end(); ++it )
	{
		size_t dist = levenshtein_distance(partial_tag, it->first);
		if(dist < nearest)
		{
			nearest = dist;
			nearest_str = it->first;
		}
	}

	return nearest_str;
}
