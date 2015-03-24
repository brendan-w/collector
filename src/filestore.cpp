

#include <string>
#include <iostream>
#include <algorithm>
#include <stdio.h>

#include "config.h"
#include "utils.h"
#include "selector.h"
#include "file.h"
#include "filestore.h"



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
	
	/*
	for(auto it: tags)
	{
		std::cout << it.first << std::endl;
	}
	*/

	std::cout << "Total: " << files.size() << std::endl;
	std::cout << "Total Tags: " << tags.size() << std::endl;
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


tag_set FileStore::auto_complete(const std::string & partial_tag)
{
	tag_set result;
	result.insert(fuzzy_match(partial_tag));
	return result;
}

file_set* FileStore::select(Selector* selector)
{
	//file_set* result = new file_set;
	Tag_operations ops = selector->get_operations();

	for(Tag_operation* op: ops)
	{
		std::cout << op->get_tag() << " ";
	}

	std::cout << std::endl;

	//return result;
	delete selector;
	return NULL;
}





file_set FileStore::set_for_tag(const std::string & tag)
{
	tag_map::const_iterator result = tags.find(tag);

	file_set set;
	if(result != tags.end())
		set = result->second;

	return set;
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	tag_set file_tags = tags_for_file(file);

	for(std::string t: file_tags)
	{
		//if it's not in the set, add it. (faster than .emplace())
		if(tags.find(t) == tags.end())
			tags[t].insert(file);
	}
}



//extracts tags from the file's path and name
//splits a string on multiple delimeters
tag_set FileStore::tags_for_file(File* file)
{
	tag_set tags;
	std::string path = file->get_path();

	// to_lower(path);

	size_t prev = 0;
	size_t pos = 0;

	//while there is another delimeter
    while((pos = path.find_first_of(config->tag_delim, prev)) != std::string::npos)
    {
        if(pos > prev)
        {
            tags.insert(path.substr(prev, pos-prev));
        }
        prev = pos + 1;
    }

    //add the last tag to the set
    if(prev < path.length())
    {
        tags.insert(path.substr(prev, std::string::npos));
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
