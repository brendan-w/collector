

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

void File::add_tag(Tag_Entry* t)
{
	//calculate where to move this file
	//search for a subdirectory to place the file in

	//split the filepath into directories and file name
	std::string dirs = get_path();
	std::string name = dirs;

	size_t last_dir = dirs.rfind(PATH_SEP);

	if(last_dir != std::string::npos)
	{
		last_dir++; //include the PATH_SEP in the dirs portion, and not the name
		dirs = dirs.substr(0, last_dir);
		name = name.substr(last_dir);
	}
	else
	{
		dirs = "";
	}

	std::string dir_tag = path_join(dirs, t->tag);

	std::string dest = "";

	if(dir_exists(path_join(config->cwd_path, dir_tag).c_str()))
	{
		//there is a subdirectory to encode this tag
		dest = path_join(dir_tag, name);
	}
	else
	{
		//there is no subdirectory for this tag, add it to the filename
		dest = path_join(dirs, t->tag + config->default_tag_delim + name);
	}

	//make it absolute from the root
	dest = path_join(config->cwd_path, dest);

	//move the file (handles possible collisions)
	move(dest);

	//add this file to the tag entry
	t->files.insert(this);

	//mark the this file with the tag entry
	tags.insert(t);
}

void File::remove_tag(Tag_Entry* t)
{


	//remove this file from the tag entry
	t->files.erase(this);

	//unmark the this file with the tag entry
	tags.erase(t);
}

void File::move(std::string dest)
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
		return;
	}

	//update the path for this file
	size_t root_path_length = config->cwd_path.length() + 1;
	path = dest.substr(root_path_length, std::string::npos);
}
