

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> //rename()
#include <algorithm> //replace()

#include <collector.h> //config->cwd_path, config->open_cmd
#include <utils.h> //escape_file_name()
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

void File::open(std::string filepath)
{
	filepath = escape_file_name(filepath);
	std::string open_cmd = config->open_cmd + filepath;
	system(open_cmd.c_str());
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

size_t File::get_size()
{
	std::string whole_path = path_join(config->cwd_path, path);
	std::ifstream fin(whole_path.c_str(), std::ifstream::ate | std::ifstream::binary);
	return fin.tellg();
}

bool File::has_tag(Tag_Entry* t)
{
	return (tags.find(t) != tags.end());
}

tag_set File::compute_tags()
{
	return split_tags(path);
}

tag_set File::split_tags(std::string p)
{
	tag_set tags;

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

Path_Parts File::get_path_parts()
{
	//split the filepath into directories and file name
	Path_Parts r;
	r.dirs = "";
	r.name = get_path();

	size_t last_dir = r.name.rfind(PATH_SEP);

	if(last_dir != std::string::npos)
	{
		last_dir++; //include the PATH_SEP in the dirs portion, and not the name
		r.dirs = r.name.substr(0, last_dir);
		r.name = r.name.substr(last_dir);
	}

	return r;
}

void File::add_tag(Tag_Entry* t)
{
	//calculate where to move this file
	//search for a subdirectory to place the file in

	//split the filepath into directories and file name
	Path_Parts p = get_path_parts();

	std::string dest = "";
	std::string dir_tag = path_join(p.dirs, t->tag);

	if(dir_exists(path_join(config->cwd_path, dir_tag).c_str()))
	{
		//there is a subdirectory to encode this tag
		dest = path_join(dir_tag, p.name);
	}
	else
	{
		//there is no subdirectory for this tag, add it to the filename
		dest = path_join(p.dirs, t->tag + config->default_tag_delim + p.name);
	}

	//make it absolute from the root
	dest = path_join(config->cwd_path, dest);

	//move the file (handles possible collisions)
	if(move(dest))
	{
		//if the file move was successful

		//add this file to the tag entry
		t->files.insert(this);

		//mark the this file with the tag entry
		tags.insert(t);
	}
}

void File::remove_tag(Tag_Entry* t)
{
	//split the filepath into directories and file name
	Path_Parts p = get_path_parts();





	std::string dest = "";

	//remove this file from the tag entry
	t->files.erase(this);

	//unmark the this file with the tag entry
	tags.erase(t);
}

bool File::move(std::string dest)
{
	//look for a collision
	if(file_exists(dest.c_str()))
	{
		//as long as there's a collision, try adding "(i)" to the filename

		size_t ext_pos = dest.rfind(".");
		std::string pathname = dest.substr(0, ext_pos);
		std::string ext = dest.substr(ext_pos);

		size_t i = 1;
		std::string new_dest;

		do
		{
			new_dest = pathname + "(" + std::to_string(i) + ")" + ext;
			i++;
		}
		while(file_exists(new_dest.c_str()));

		dest = new_dest;
	}

	std::cout << dest << std::endl;

	//try the rename
	if(rename(get_full_path().c_str(), dest.c_str()))
	{
		//failure
		perror(path.c_str());
		return false;
	}
	else
	{
		//update the path for this file
		size_t root_path_length = config->cwd_path.length() + 1;
		path = dest.substr(root_path_length, std::string::npos);
		return true;
	}
}
