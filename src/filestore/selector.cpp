

#include <string>

#include <filestore/selector.h>
#include <utils.h>


/*
	Tag_operation object
*/

Tag_operation::Tag_operation(std::string input)
{
	if(input.length() > 0)
	{
		to_lower(input);

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
