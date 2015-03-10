

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>

#include "filestore.h"
#include "file.h"
#include "collector.h"


FileStore::FileStore()
{
	//fill the files vector with all valid file paths found
	exec_find();

	std::cout << "done: " << files.size() << std::endl;
	std::cout << "bin path: " << config->get_bin_path() << std::endl;
	std::cout << "cwd path: " << config->get_cwd_path() << std::endl;
}


FileStore::~FileStore()
{
	for(size_t i = 0; i < files.size(); i++)
	{
		delete files[i];
	}
	files.clear();
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
			files.push_back(new File(path));
		}
	}

	pclose(pipe);
}
