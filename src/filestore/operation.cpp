

#include <string>

#include <utils.h>
#include <filestore/operation.h>




Operation::Operation(std::string t, Selection_operation o)
{
	to_lower(t);
	tag = t;
	op = o;
}

Operation::~Operation()
{
	
}
