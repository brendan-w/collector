

#include <string>
#include <iostream>
#include <stdio.h>

#include "filestore.h"
#include "file.h"
#include "collector.h"


FileStore::FileStore()
{
	//fill the files vector with all valid file paths found
	exec_find();

	for(File* file: files)
	{
		//get all tags, relative to the current working directory
		tag_set file_tags = get_tags(file);

		for(std::string t: file_tags)
		{
			tags[t].insert(file);
		}
	}

	for(auto it: tags)
	{
		std::cout << it.first << std::endl;
	}
}


FileStore::~FileStore()
{
	for(auto file_it = files.begin(); file_it != files.end(); ++file_it)
	{
		delete *file_it;
	}
	files.clear();
	tags.clear();
}


//extracts tags from the file's path and name
//splits a string on multiple delimeters
tag_set FileStore::get_tags(File* file)
{
	tag_set tags;
	std::string path = file->get_file_path();

	size_t prev = 0;
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


void FileStore::exec_find()
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
			files.insert(new File(path));
		}
	}

	pclose(pipe);
}
