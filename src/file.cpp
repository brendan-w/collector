

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "texture.h"
#include "file.h"
#include "collector.h"



File::File(std::string file_path)
{
	path = file_path;
	texture = NULL;

	// std::cout << "path: " << path << std::endl;

}

File::~File()
{
	if(texture != NULL)
	{
		delete texture;
	}
}

std::unordered_set<std::string> File::get_tags()
{
	std::unordered_set<std::string> tags;

	std::cout << "=================" << std::endl;
	std::cout << path << std::endl;

	size_t prev = 0;
	size_t pos = 0;
    while((pos = path.find_first_of(config->get_tag_delim(), prev)) != std::string::npos)
    {
        if(pos > prev)
            tags.insert(path.substr(prev, pos-prev));
        prev = pos+1;
    }
    if(prev < path.length())
        tags.insert(path.substr(prev, std::string::npos));

    for(auto it = tags.begin(); it != tags.end(); ++it)
    {
		std::cout << *it << std::endl;
    }

	return tags;
}

std::string File::get_file_path()
{
	return path;
}

void File::set_file_path(std::string new_path)
{

}
