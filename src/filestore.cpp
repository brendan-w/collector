

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

#include "config.h"
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
	return result;
}

file_set FileStore::query(const std::string & tag)
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
	tag_set file_tags = get_tags(file);

	for(std::string t: file_tags)
	{
		//if it's not in the set, add it. (faster than .emplace())
		if(tags.find(t) == tags.end())
			tags[t].insert(file);
	}
}

//extracts tags from the file's path and name
//splits a string on multiple delimeters
tag_set FileStore::get_tags(File* file)
{
	tag_set tags;
	std::string path = file->get_path();

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


unsigned int FileStore::levenshtein_distance(const std::string & s1, const std::string & s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2+1), prevCol(len2+1);

	for(unsigned int i = 0; i < prevCol.size(); i++)
	{
		prevCol[i] = i;
	}

	for(unsigned int i = 0; i < len1; i++)
	{
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++)
		{
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1),
								prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}
