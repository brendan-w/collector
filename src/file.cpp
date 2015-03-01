

#include <string>
#include "texture.h"
#include "file.h"


File::File(std::string path)
{
	path = path;
	texture = NULL;
}

File::~File()
{
	if(texture != NULL)
	{
		delete texture;
	}
}