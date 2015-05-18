

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h> //rename()
#include <algorithm> //replace()

#include <collector.h> //config->cwd_path, config->open_cmd
#include <utils.h> //escape_file_name(), get_path_parts(), join_path_parts()
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

std::string File::get_exemplar_tag()
{
	std::string tag = "";
	std::string p = path;
	size_t last_dir = p.rfind(PATH_SEP);

	if(last_dir != std::string::npos)
	{
		p = p.substr(0, last_dir);
		last_dir = p.rfind(PATH_SEP);
		
		if(last_dir == std::string::npos)
			tag = p;
		else
			tag = p.substr(last_dir + 1);
	}

	return tag;
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

void File::add_tag(Tag_Entry* t)
{
	//calculate where to move this file
	//search for a subdirectory to place the file in

	//split the filepath into directories and file name
	Path_Parts p = get_path_parts(path);

	std::string dest = "";
	std::string dir_tag = path_join(p.dirs, t->tag);

	if(dir_exists(path_join(config->cwd_path, dir_tag).c_str()))
	{
		//there is a subdirectory to encode this tag
		p.dirs = dir_tag;
	}
	else
	{
		//there is no subdirectory for this tag, add it to the filename
		p.name += config->default_tag_delim + t->tag;
	}

	dest = join_path_parts(p);

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
	Path_Parts p = get_path_parts(path);

	const size_t tag_len = t->tag.length();

	tag_set dir_tags  = split_tags(p.dirs);
	tag_set name_tags = split_tags(p.name);


	/*
		if the name carries this tag

		note: name does not include the extension
		file extensions can be searched, but not modified
	*/
	if(name_tags.find(t->tag) != name_tags.end())
	{
		//remove all instances of it

		//operate on a lower case version of the string
		//to preserve the case of the original
		std::string lower_name = p.name;
		to_lower(lower_name);

		size_t pos;
		while( (pos = lower_name.find(t->tag)) != std::string::npos )
		{
			lower_name.erase(pos, tag_len);
			p.name.erase(pos, tag_len);

			//remove the delimeter
			if(pos >= p.name.length())
			{
				//this tag was at the end of the filename
				p.name.erase(pos - 1, 1);
			}
			else
			{
				//this tag was NOT at the end of the filename
				//erase the forward delimeter
				p.name.erase(pos, 1);
			}
		}

		//keep the tag_set up to date, so that the directory handler can check for duplicates
		name_tags.erase(t->tag);

		//prevent files with no names
		if(p.name.length() == 0)
		{
			//TODO: update the FileStore for these outcomes
			p.name = "unknown";
		}
	}


	/*
		a directory carries this tag
	*/
	if(dir_tags.find(t->tag) != dir_tags.end())
	{
		//operate on a lower case version of the string
		//to preserve the case of the original
		std::string lower_dirs = p.dirs;
		to_lower(lower_dirs);
		
		//find the first occurence of this tag in the directory path
		size_t dir_pos = lower_dirs.find(t->tag);

		//scan backwards to find the preceeding PATH_SEP
		dir_pos = lower_dirs.rfind(PATH_SEP, dir_pos);
		if(dir_pos == std::string::npos)
			dir_pos = 0;

		//when the file gets popped out of a directory,
		//it may loose more tags than just the deleted tag.
		//get the string of all dirs that are about to go away
		std::string path_casualties = p.dirs.substr(dir_pos);

		//pop the file up to the appropriate dir
		p.dirs = p.dirs.substr(0, dir_pos);

		//encode the casualty tags in the filename
		tag_set casualty_tags = split_tags(path_casualties);

		//exclude the tag that we're trying to remove
		casualty_tags.erase(t->tag);

		for(std::string tag: casualty_tags)
		{
			//if this tag wasn't already in the filename, add it
			if(name_tags.find(tag) == name_tags.end())
				p.name = tag + config->default_tag_delim + p.name;
		}
	}

	//put the parts back together
	std::string dest = join_path_parts(p);

	//make it absolute from the root
	dest = path_join(config->cwd_path, dest);

	// std::cout << dest << std::endl;

	//move the file (handles possible collisions)
	if(move(dest))
	{
		//if the file move was successful

		//remove this file from the tag entry
		t->files.erase(this);

		//unmark the this file with the tag entry
		tags.erase(t);
	}
}

bool File::move(std::string dest)
{
	//look for a collision
	if(file_exists(dest.c_str()))
	{
		//as long as there's a collision, try adding "(i)" to the filename
		Path_Parts p = get_path_parts(dest);

		size_t i = 1;
		std::string new_dest;

		do
		{
			Path_Parts new_p = p;
			p.name += "(" + std::to_string(i) + ")";
			new_dest = join_path_parts(new_p);
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
