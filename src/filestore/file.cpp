

#include <string>
#include <fstream>
#include <algorithm> //replace()

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

std::string File::get_link_path()
{
	std::string link = path;
	std::replace(link.begin(), link.end(), PATH_SEP, '_');
	return path_join(config->export_path, link);
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
