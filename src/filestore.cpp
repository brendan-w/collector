

#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "filestore.h"


FileStore::FileStore()
{
	// root = "~/cool";
	root = ".";

	std::vector<std::string> files;
	exec_find("*", files);

	std::cout << "done: " << files.size() << std::endl;
}


FileStore::~FileStore()
{

}


void FileStore::exec_find(std::string query, std::vector<std::string> &lines)
{
	std::string cmd = "find " + root + " -name \"" + query + "\"";

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
