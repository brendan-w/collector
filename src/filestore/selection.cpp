
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# selection.cpp                                                        #
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


#include <string>
#include <string.h>
#include <iostream>
#include <cstdlib> //system()
#include <unistd.h> //symlink()
#include <algorithm> //sort()
#include <sys/stat.h> //mkdir()

#include <collector.h> //config
#include <utils.h> //is_number(), starts_with(), set_union(), path_join(), PATH_SEP
#include <filestore/file.h>
#include <filestore/selection.h>



static bool tag_entry_compare(TagEntry* a, TagEntry* b)
{
	return a->files.size() > b->files.size();
}

static bool file_compare(File* a, File* b)
{
	return strcmp(a->get_path().c_str(), b->get_path().c_str()) > 0;
}


Selection::Selection(FileVector* all, FileSet fs) : all_files(all)
{
	exported = false;
	files = fs;

	/*
		Files
	*/

	//create the sorted file vector
	for(File* f: files)
		files_sorted.push_back(f);

	std::sort(files_sorted.begin(),
	          files_sorted.end(),
	          file_compare);

	/*
		Tags
	*/

	//compile the entry set
	EntrySet entries;
	for(File* f: files)
		set_union(entries, f->tags);

	//filter and dump the set to a sortable vector of entries
	//fill the subtags set with plain-text tag names
	EntryVector entries_sorted;
	for(TagEntry* entry: entries)
	{
		//strain out lone subtags
		if(entry->files.size() == 1)
			continue;

		//strain out numbers
		if(is_number(entry->tag))
			continue;

		entries_sorted.push_back(entry);
		subtags.insert(entry->tag);
	}

	//sort in descending number of files per tag
	std::sort(entries_sorted.begin(),
			  entries_sorted.end(),
			  tag_entry_compare);

	//convert sorted tags into plain-text tags
	for(TagEntry* entry: entries_sorted)
		subtags_sorted.push_back(entry->tag);
}

Selection::~Selection()
{
	files.clear();
	files_sorted.clear();

	subtags.clear();
	subtags_sorted.clear();
}


void Selection::export_()
{
	if(!exported)
	{
		//remains silent when directory already exists
		mkdir(config->export_path.c_str(), 0777);

		std::string rm_cmd = "exec rm -r " + path_join(config->export_path, "*");
		system(rm_cmd.c_str());

		for(File* file: files)
		{
			//compute the name of the symlink
			symlink(file->get_full_path().c_str(),
					file->get_link_path().c_str());
		}

		exported = true;
	}
}

bool Selection::has(File* file)
{
	return (files.find(file) != files.end());
}

File* Selection::all_at(size_t i)
{
	if(i < all_size())
		return (*all_files)[i];
	else
		return NULL;
}

bool Selection::has_subtag(std::string tag)
{
	return (subtags.find(tag) != subtags.end());
}

/*
	returns the completion for a given partial tag:

	given:    "part"
	real tag: "partial"
	returns:      "ial"
*/
std::string Selection::auto_complete(std::string partial)
{
	for(std::string tag: subtags)
	{
		if(starts_with(tag, partial))
			return tag.substr(partial.length());
	}

	return "";
}
