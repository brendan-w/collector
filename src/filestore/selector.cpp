

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
				tag_intersections.push_back(tag);
				break;
			case EXCLUSION:
				tag_exclusions.push_back(tag);
				break;
			case UNION:
				break;
		}
	}
}

bool Selector::is_empty()
{
	return (tag_intersections.size() + tag_exclusions.size()) == 0;
}
