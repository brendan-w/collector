

#include <string>

#include "selector.h"


/*
	Tag_operation object
*/

Tag_operation::Tag_operation(std::string input)
{
	tag = "";
	op = NONE;

	if(input.length() > 0)
	{
		//the first character may hold an operation indicator
		char c = input[0];

		switch(c)
		{
			case UNION:
			case EXCLUSION:
				tag = input.substr(1, std::string::npos);
				op = (Set_operation) c;
				break;
			default:
				//tags without specified set operations
				tag = input;
				op = INTERSECTION;
				break;
		}
	}
}

Tag_operation::~Tag_operation()
{

}

std::string Tag_operation::get_tag()
{
	return tag;
}

Set_operation Tag_operation::get_op()
{
	return op;
}



/*
	Selector object
*/

Selector::Selector()
{

}

Selector::~Selector()
{
	for(Tag_operation* tag: tags)
	{
		delete tag;
	}
	tags.clear();
}

void Selector::add_operation(std::string input)
{
	if(input.length() > 0)
	{
		Tag_operation* tag = new Tag_operation(input);
		tags.push_back(tag);
	}
}

const Tag_operations Selector::get_operations()
{
	return tags;
}
