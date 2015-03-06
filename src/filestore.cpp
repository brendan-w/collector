

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "filestore.h"
#include "file.h"


FileStore::FileStore()
{
	std::vector<std::string> file_paths;
	exec_find(file_paths);

	std::cout << "done: " << file_paths.size() << std::endl;

	//instantiate file objects for each filepath
	for(unsigned int i = 0; i < file_paths.size(); i++)
	{
		files.push_back(new File(file_paths[i]));
	}
}


FileStore::~FileStore()
{
	for(unsigned int i = 0; i < files.size(); i++)
	{
		delete files[i];
	}
}


void FileStore::exec_find(std::vector<std::string> &lines)
{
	//lists all items in the current directory and below
	//only returns files
	//excludes dot-files and dot-folders
	//excludes executables where o=x
	std::string cmd = "find . -type f \\( -path \"*\" ! -path \"*/.*\" ! -perm -o=x \\)";

	FILE* pipe = popen(cmd.c_str(), "r");

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
			lines.push_back(path);
		}
	}

	pclose(pipe);
}
