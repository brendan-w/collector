
#include <iostream>
#include <string>
#include <stdio.h>

#include <collector.h>
#include <utils.h> //set_union(), set_intersect()
#include <filestore/types.h>
#include <filestore/filestore.h>



FileStore::FileStore()
{
	//run command to return /n delimited list of files in the current directory
	FILE* pipe = popen(config->find_cmd.c_str(), "r");

	if(!pipe)
		return;

	char *line = NULL;
	size_t size = 0;
	size_t root_path_length = config->cwd_path.length() + 1;

	while(!feof(pipe))
	{
		if(getline(&line, &size, pipe) != -1)
		{
			std::string path = std::string(line);
			
			//pop the ending newline
			path.pop_back();

			//remove root working directory
			path = path.substr(root_path_length, std::string::npos);

			//create a new File object, and save it in the vector
			insert_file(new File(path));
		}
	}

	pclose(pipe);
}


FileStore::~FileStore()
{
	for(File* file: files)
	{
		delete file;
	}

	files.clear();
	tags.clear();
}

//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	//result holders
	file_set r_files;
	entry_set r_subtags;

	if(!selector->is_empty())
	{
		tag_vector intersections = selector->get_tag_intersections();

		bool first = true;
		for(std::string tag: intersections)
		{
			//prevent unknown tags from destroying the query
			if(!has_tag(tag))
				continue;

			Tag_Entry* entry = tags[tag];

			if(first)
			{
				//the first tag to be processed is a subset of the universe
				r_files = entry->files;
				first = false;
			}
			else
			{
				file_set r_files_intersect;
				set_intersect<file_set>(r_files_intersect,
										r_files,
										entry->files);

				//if the intersection created a null set, disregard this tag
				if(r_files_intersect.size() > 0)
				{
					r_files = r_files_intersect;
				}
			}
		}

		//process the manually included and excluded files
		file_map_bool inexclude = selector->get_inexclude();
		for(auto e: inexclude)
		{
			if(e.second)
				r_files.insert(e.first); //include this file
			else
				r_files.erase(e.first); //exclude this file
		}

		//compute the set of subtags by performing a union
		for(File* file: r_files)
		{
			set_union(r_subtags, file->tags);
		}
	}

	//done selecting
	delete selector;
	return new Selection(&files, r_files, r_subtags);
}


void FileStore::operation(Selection* selection, Operation* operation)
{
	switch(operation->get_op())
	{
		case ADD_TAG:
			for(File* file: *selection)
			{
				add_tag_on_file(file, operation->get_tag());
			}
			break;

		case REMOVE_TAG:
		{
			std::string tag = operation->get_tag();
			
			if(selection->has_subtag(tag))
			{
				for(File* file: *selection)
					remove_tag_on_file(file, tag);
			}
			
			break;
		}

		case DELETE_FILES:
			break;
	}
}


bool FileStore::has_tag(const std::string & tag)
{
	return (tags.find(tag) != tags.end());
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	tag_set file_tags = tags_for_file(file);


	//first iteration, populate the tag map with any new tags
	for(std::string tag: file_tags)
	{
		Tag_Entry* entry = NULL;

		if(!has_tag(tag))
		{
			//create a new entry object for this tag
			entry = new Tag_Entry;
			entry->tag = tag;
			entry->files.insert(file);

			tags[tag] = entry;
		}
		else
		{
			//add the file to the correct tag file_set
			entry = tags[tag];
			entry->files.insert(file);
		}

		//give the file a pointer to each of its Tag_Entry
		file->tags.insert(entry);
	}
}


tag_set FileStore::tags_for_file(File* file)
{
	tag_set tags;

	//copy the path, so to_lower won't affect the original
	std::string p = file->get_path();
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


void FileStore::add_tag_on_file(File* file, const std::string & tag)
{

}

void FileStore::remove_tag_on_file(File* file, const std::string & tag)
{

}
