

#include <string>

#include <filestore/tag_info.h>


Tag_Info::Tag_Info(std::string partial)
{
	partial_tag = partial;
}

Tag_Info::~Tag_Info()
{

}

std::string Tag_Info::get_partial()
{
	return partial_tag;
}

std::string Tag_Info::get_completed()
{
	return complete_tag;
}

void Tag_Info::set_completed(std::string complete)
{
	complete_tag = complete;
}
