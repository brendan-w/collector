

#include <string>

#include <filestore/selector.h>
#include <utils.h>


/*
	Selector object
*/

Selector::Selector()
{

}

Selector::~Selector()
{
	tag_intersections.clear();
	tag_exclusions.clear();
	file_include.clear();
	file_exclude.clear();
}

void Selector::add_operation(std::string tag, Set_operation op)
{
	if(tag.length() > 0)
	{
		switch(op)
		{
			case INTERSECTION:
				tag_intersections.insert(tag);
				break;
			case EXCLUSION:
				tag_exclusions.insert(tag);
				break;
			case UNION:
				break;
		}
	}
}

//used by the Selection object to avoid listing subtags
//that are gauranteed to be in the result (the user
//already knows about them)
bool Selector::has(std::string tag)
{
	return tag_intersections.find(tag) != tag_intersections.end();
}

bool Selector::is_empty()
{
	return (tag_intersections.size() + tag_exclusions.size()) == 0;
}
