

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include "texture.h"
#include "file.h"
#include "filestore.h"



File::File(std::string file_path)
{
	path = file_path;
	texture = NULL;
}

File::~File()
{
	if(texture != NULL)
	{
		delete texture;
	}
}

std::string File::get_path()
{
	return path;
}

void File::set_path(std::string new_path)
{

}
