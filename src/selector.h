
#pragma once

#include <string>
#include <vector>


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
		std::string get_tag();
		Set_operation get_op();
	private:
		std::string tag;
		Set_operation op;
};


typedef std::vector<Tag_operation*> Tag_operations;


class Selector
{
	public:
		Selector();
		~Selector();
		void add_operation(std::string input);
		const Tag_operations get_operations();
	private:
		Tag_operations tags;
};
