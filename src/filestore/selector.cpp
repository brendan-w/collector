

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
	inexclude.clear();
}

void Selector::add_tag(std::string tag, Set_Operation op)
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

void Selector::add_inexclude(File* f, bool included)
{
	inexclude[f] = included;
}

bool Selector::is_empty()
{
	return (tag_intersections.size() + tag_exclusions.size() + inexclude.size()) == 0;
}
