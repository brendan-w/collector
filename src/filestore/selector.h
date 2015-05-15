
#pragma once

#include <string>
#include <vector>

#include <filestore/file.h>


typedef std::unordered_map<File*, bool> file_map_bool;


enum Set_operation
{
	INTERSECTION = 0,
	EXCLUSION,
	UNION
};


class Selector
{
	public:
		Selector();
		~Selector();

		void add_operation(std::string tag, Set_operation op);
		void add_inexclude(File* f, bool included);

		const tag_vector get_tag_intersections() { return tag_intersections; }
		const tag_vector get_tag_exclusions()    { return tag_exclusions; }
		const file_map_bool get_inexclude()      { return inexclude; }

		bool is_empty();

	private:
		tag_vector tag_intersections;
		tag_vector tag_exclusions;
		file_map_bool inexclude;
};
