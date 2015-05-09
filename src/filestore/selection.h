
#pragma once

#include <utility>

#include <filestore/file.h>
#include <filestore/selector.h>


typedef std::unordered_map<std::string, size_t> tag_freq_set;
typedef std::pair<std::string, size_t> tag_freq;
typedef std::vector<tag_freq> tag_freq_vector;


class Selection
{
	public:
		Selection(Selector* s, file_vector* all, file_set fs);
		~Selection();

		bool has(File* file);
		tag_freq_vector get_subtags() { return subtags; }

		//the selection
		const file_set_it begin() { return files.begin(); }
		const file_set_it end()   { return files.end(); }
		size_t size()       { return files.size(); }
		File* at(size_t i);

		//all files
		const file_vector_it all_begin() { return all_files->begin(); }
		const file_vector_it all_end()   { return all_files->end(); }
		size_t all_size()          { return all_files->size(); }
		File* all_at(size_t i);

	private:
		 //set of Files selected by the previous Selector
		file_set files;
		
		//the whole file list
		file_vector* all_files;

		//the selector that generated this selection
		Selector* selector;

		//subtags
		tag_freq_vector subtags;

		//functions
		void load_subtags();
};
