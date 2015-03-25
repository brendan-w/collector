

#include <string>
#include <iostream>
#include <algorithm>
#include <stdio.h>

#include "config.h"
#include "utils.h"
#include "selector.h"
#include "selection.h"
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


//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	Selection* selection = new Selection;

	/*
	Tag_operations ops = selector->get_operations();
	for(Tag_operation* op: ops)
	{
		std::cout << op->get_tag() << " ";
	}
	std::cout << std::endl;
	*/

	//dumb test selector
	Tag_operations ops = selector->get_operations();

	if(ops.size() > 0)
	{
		std::string tag = ops[0]->get_tag();
		tag = fuzzy_match(tag);

		std::cout << tag << " : " << tags[tag].size() << std::endl;

		for(auto it = tags[tag].begin(); it != tags[tag].end(); ++it)
		{
			selection->add_file(*it);
		}
	}

	delete selector;
	return selection;
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
	tag_set file_tags = file->get_tags();

	for(std::string t: file_tags)
	{
		//add the file to the correct tag file_set
		tags[t].insert(file);
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
