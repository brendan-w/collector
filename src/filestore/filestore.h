
#pragma once


#include <string>
#include <vector>
#include <unordered_map>

#include <filestore/types.h>


class Tag_Entry
{
	public:
		std::string tag;
		file_set files;
		std::unordered_set<Tag_Entry*> subtags;
};

typedef std::vector<Tag_Entry*> entry_vector;
typedef std::unordered_set<Tag_Entry*> entry_set;
typedef std::unordered_map<std::string, Tag_Entry*> tag_map;



class FileStore
{
	public:
		FileStore();
		~FileStore();

		//primary accessors
		Tag_Info* tag_info(Tag_Info* c);
		Selection* select(Selector* selector);

	private:
		file_vector files;
		tag_map tags;

		bool has_tag(const std::string & tag);
		file_set set_for_tag(const std::string &tag);
		void insert_file(File* file);
		std::string fuzzy_match(const std::string & partial_tag);
};
