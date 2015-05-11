

#include <string>
#include <fstream>


#include <collector.h>
#include <utils.h>
#include <thumbnail.h>
#include <filestore/file.h>



File::File(std::string file_path)
{
	path = file_path;
	thumb = NULL;
}

File::~File()
{
	unload();
}

void File::unload()
{
	if(thumb != NULL)
	{
		delete thumb;
		thumb = NULL;
	}
}

Thumbnail* File::get_thumb()
{
	if(thumb == NULL)
		thumb = new Thumbnail(get_full_path());
	return thumb;
}

std::string File::get_path()
{
	return path;
}

std::string File::get_full_path()
{
	return path_join(config->cwd_path, path);
}

void File::set_path(std::string new_path)
{
	path = new_path;
}

size_t File::get_size()
{
	std::string whole_path = path_join(config->cwd_path, path);
	std::ifstream fin(whole_path.c_str(), std::ifstream::ate | std::ifstream::binary);
	return fin.tellg();
}
