

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

//extracts tags from the file's path and name
//splits a string on multiple delimeters
str_tag_set File::get_tags()
{
	str_tag_set tags;

	//copy the path, so to_lower won't affect the original
	std::string p = path;
	to_lower(p);

	size_t prev = 0;
	size_t pos = 0;

	//while there is another delimeter
    while((pos = p.find_first_of(config->tag_delim, prev)) != std::string::npos)
    {
        if(pos > prev)
        {
            tags.insert(p.substr(prev, pos-prev));
        }
        prev = pos + 1;
    }

    //add the last tag to the set
    if(prev < p.length())
    {
        tags.insert(p.substr(prev, std::string::npos));
    }

	return tags;
}

size_t File::get_size()
{
	std::string whole_path = path_join(config->cwd_path, path);
	std::ifstream fin(whole_path.c_str(), std::ifstream::ate | std::ifstream::binary);
	return fin.tellg();
}
