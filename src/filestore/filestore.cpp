
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# filestore.cpp                                                        #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/

#include <iostream>
#include <string>
#include <stdio.h> //rename

#include <collector.h>
#include <utils.h> //set_union(), set_intersect(), dir_exists(), PATH_SEP
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
		delete file;

	for(auto e: tags)
		delete e.second; //the TagEntry

	files.clear();
	tags.clear();
}

//turns Selectors into Selections
Selection* FileStore::select(Selector* selector)
{
	//result holders
	FileSet r_files;

	if(!selector->is_empty())
	{
		/*
			Intersections
		*/

		bool first = true;
		for(Tag tag: selector->get_tag_intersections())
		{
			//prevent unknown tags from destroying the query
			if(!has_tag(tag))
				continue;

			TagEntry* entry = tags[tag];

			if(first)
			{
				//the first tag to be processed is a subset of the universe
				r_files = entry->files;
				first = false;
			}
			else
			{
				FileSet r_files_intersect;
				set_intersect<FileSet>(r_files_intersect,
										r_files,
										entry->files);

				//if the intersection created a null set, disregard this tag
				if(r_files_intersect.size() > 0)
				{
					r_files = r_files_intersect;
				}
			}
		}

		/*
			Exclusions
		*/

		for(Tag tag: selector->get_tag_exclusions())
		{
			//prevent unknown tags from destroying the query
			if(!has_tag(tag))
				continue;

			TagEntry* entry = tags[tag];

			//exclude these files from the selection
			for(File* f: entry->files)
				r_files.erase(f);
		}

		/*
			Manual Includes & Excludes
		*/

		InExclude inexclude = selector->get_inexclude();
		for(auto e: inexclude)
		{
			if(e.second)
				r_files.insert(e.first); //include this file
			else
				r_files.erase(e.first); //exclude this file
		}
	}

	//done selecting
	delete selector;
	return new Selection(&files, r_files);
}


void FileStore::operation(Operation* operation)
{
	Selection* const selection = operation->get_selection();

	if(operation->get_delete())
	{
		//delete the selected files
		//TODO
	}
	else
	{
		//add/remove tags

		for(Tag tag: operation->get_add_tags())
			add_tag(selection, tag);

		for(Tag tag: operation->get_remove_tags())
			remove_tag(selection, tag);
	}

	delete operation;
}


bool FileStore::has_tag(const Tag & tag)
{
	return (tags.find(tag) != tags.end());
}

void FileStore::insert_file(File* file)
{
	files.push_back(file);

	//get all tags, relative to the current working directory
	TagSet file_tags = file->compute_tags();

	//first iteration, populate the tag map with any new tags
	for(std::string tag: file_tags)
	{
		TagEntry* entry = NULL;

		if(!has_tag(tag))
		{
			//create a new entry object for this tag
			entry = new TagEntry;
			entry->tag = tag;
			entry->files.insert(file);

			tags[tag] = entry;
		}
		else
		{
			//add the file to the correct tag FileSet
			entry = tags[tag];
			entry->files.insert(file);
		}

		//give the file a pointer to each of its TagEntry
		file->tags.insert(entry);
	}
}

void FileStore::add_tag(Selection* const selection, const Tag & tag)
{
	//update the data sctructure for the new tag
	TagEntry* entry;
	
	if(has_tag(tag))
	{
		entry = tags[tag];
	}
	else
	{
		//create the new TagEntry
		entry = new TagEntry;
		entry->tag = tag;
		tags[tag] = entry;
	}

	for(File* file: *selection)
	{
		if(!file->has_tag(entry))
			file->add_tag(entry);
	}
}

void FileStore::remove_tag(Selection* const selection, const Tag & tag)
{
	if(!has_tag(tag))
		return; //tag has never been seen before. Done.

	TagEntry* entry = tags[tag];

	for(File* file: *selection)
	{
		if(file->has_tag(entry))
			file->remove_tag(entry);
	}

	//delete the TagEntry if there are no remaining files with that tag
	if(entry->files.size() == 0)
	{
		tags.erase(tag);
		delete entry;
	}
}
