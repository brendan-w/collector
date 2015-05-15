
#pragma once

#include <string>
#include <vector>

#include <filestore/file.h>


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
		void include(File* f);
		void exclude(File* f);

		const tag_vector get_tag_intersections() { return tag_intersections; }
		const tag_vector get_tag_exclusions()    { return tag_exclusions; }
		const file_set get_include() { return file_include; };
		const file_set get_exclude() { return file_exclude; };

		bool is_empty();

	private:
		tag_vector tag_intersections;
		tag_vector tag_exclusions;
		file_set file_include;
		file_set file_exclude;
};
