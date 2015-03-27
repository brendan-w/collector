
#include <iostream>
#include <string>
#include <stdio.h>

#include <config.h>
#include <utils.h>
#include <filestore/selector.h>
#include <filestore/selection.h>
#include <filestore/file.h>
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


tag_set FileStore::auto_complete(const std::string & partial_tag)
{
	tag_set result;
	result.insert(fuzzy_match(partial_tag));
	return result;
}


//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	file_set result;

	if(selector != NULL)
	{
		Tag_operations ops = selector->get_operations();

		bool first = true;

		for(Tag_operation* op: ops)
		{
			if(first)
			{
				//the first tag to be processed is a subset of the universe
				result = set_for_tag(op->get_tag());
				first = false;
			}
			else
			{
				file_set copy = result;
				file_set current = set_for_tag(op->get_tag());

				intersect(result, copy, current);
			}
		}
	}

	//done selecting
	delete selector;
	return new Selection(&files, result);
}




file_set FileStore::set_for_tag(const std::string & tag)
{
	tag_map::const_iterator result = tags.find(tag);

	file_set output;
	if(result != tags.end())
		output = result->second;

	return output;
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	tag_set file_tags = file->get_tags();

	for(std::string t: file_tags)
	{
		//add the file to the correct tag file_set
		tags[t].insert(file);
	}
}


//return the tag with the nearest edit distance
std::string FileStore::fuzzy_match(const std::string & partial_tag)
{
	std::string nearest_str = "";
	size_t nearest = 0;

	//prepopulate the initial result
	auto it = tags.begin();
	nearest_str = it->first;
	nearest = levenshtein_distance(partial_tag, nearest_str);
	++it;

	for( ; it != tags.end(); ++it )
	{
		size_t dist = levenshtein_distance(partial_tag, it->first);
		if(dist < nearest)
		{
			nearest = dist;
			nearest_str = it->first;
		}
	}

	return nearest_str;
}
