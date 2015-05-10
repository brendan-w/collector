
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

		const str_tag_set get_tag_intersections() { return tag_intersections; }
		const str_tag_set get_tag_exclusions()    { return tag_exclusions; }
		const file_set get_include() { return file_include; };
		const file_set get_exclude() { return file_exclude; };

		bool has(std::string tag);

	private:
		str_tag_set tag_intersections;
		str_tag_set tag_exclusions;
		file_set file_include;
		file_set file_exclude;
};
