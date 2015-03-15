

#include <string>
#include <iostream>
#include <stdio.h>

#include "filestore.h"
#include "file.h"
#include "collector.h"


FileStore::FileStore()
{
	//run command to return /n delimited list of files in the current directory
	FILE* pipe = popen(config->get_find_cmd().c_str(), "r");

	if(!pipe)
		return;

	char *line = NULL;
	size_t size = 0;

	while(!feof(pipe))
	{
		if(getline(&line, &size, pipe) != -1)
		{
			std::string path = std::string(line);
			path.pop_back(); //pop the ending newline

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


void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	tag_set file_tags = get_tags(file);
	
	for(std::string t: file_tags)
	{
		tags[t].insert(file);
	}
	
}

//extracts tags from the file's path and name
//splits a string on multiple delimeters
tag_set FileStore::get_tags(File* file)
{
	tag_set tags;
	std::string path = file->get_file_path();

	size_t prev = config->get_cwd_path().length() + 1; //exclude root dir and path delimeter
	size_t pos = 0;

	//while there is another delimeter
    while((pos = path.find_first_of(config->get_tag_delim(), prev)) != std::string::npos)
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
