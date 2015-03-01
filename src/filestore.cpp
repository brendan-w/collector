

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdio.h>
#include "filestore.h"


FileStore::FileStore()
{
	root = "~/cool";

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

	char buffer[128];
	std::string result = "";

	while(!feof(pipe))
	{
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}

	pclose(pipe);

	//split response into lines

    std::stringstream ss(result);
    std::string item;
    while(std::getline(ss, item))
    {
        lines.push_back(item);
    }
}
