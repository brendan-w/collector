
#include <iostream>
#include <string>
#include <stdio.h>

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


//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	//result holders
	file_set r_files;
	tag_set r_subtags;

	if(selector != NULL)
	{
		str_tag_set intersections = selector->get_tag_intersections();

		bool first = true;
		for(std::string tag: intersections)
		{
			//prevent unknown tags from destroying the query
			if(has_tag(tag))
			{
				if(first)
				{
					//the first tag to be processed is a subset of the universe
					r_files = tags[tag]->files;
					first = false;
				}
				else
				{
					file_set copy = r_files;
					file_set current = tags[tag]->files;
					set_intersect(r_files, copy, current);
				}
			}
		}
	}

	//done selecting
	return new Selection(selector, &files, r_files);
}


bool FileStore::has_tag(const std::string & tag)
{
	return (tags.find(tag) != tags.end());
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	str_tag_set file_tags = file->get_tags();


	//first iteration, populate the tag map with any new tags
	for(std::string tag: file_tags)
	{
		if(!has_tag(tag))
		{
			//create a new entry object for this tag
			Tag_Entry* entry = new Tag_Entry;
			entry->tag = tag;
			entry->files.insert(file);

			tags[tag] = entry;
		}
		else
		{
			//add the file to the correct tag file_set
			tags[tag]->files.insert(file);
		}
	}

	//get convert every string to a pointer to an entry
	std::vector<Tag_Entry*> entries;

	for(std::string tag: file_tags)
	{
		entries.push_back(tags[tag]);
	}

	//complete graph to populate subtags

	for(size_t a = 0; a < entries.size(); a++)
	{
		// +1 prevents tags from being subtags of themselves
		for(size_t b = a+1; b < entries.size(); b++)
		{
			entries[a]->subtags.insert(entries[b]);
			entries[b]->subtags.insert(entries[a]);
		}
	}
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
