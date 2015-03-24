
#pragma once

#include <string>
#include <vector>

#include "file.h"


enum Set_operation
{
	NONE = 0,
	INTERSECTION = 1,
	UNION = '+',
	EXCLUSION = '-'
};


class Tag_operation
{
	public:
		Tag_operation(std::string input);
		~Tag_operation();
		std::string get_tag() { return tag; };
		Set_operation get_op() { return op; };

	private:
		std::string tag = "";
		Set_operation op = NONE;
};


typedef std::vector<Tag_operation*> Tag_operations;


class Selector
{
	public:
		Selector();
		~Selector();
		void add_operation(std::string input);

		const Tag_operations get_operations() { return tags; };
		const file_set get_include() { return include; };
		const file_set get_exclude() { return exclude; };

	private:
		Tag_operations tags;
		file_set include;
		file_set exclude;
};
