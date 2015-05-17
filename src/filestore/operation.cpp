

#include <string>

#include <filestore/operation.h>




Operation::Operation(std::string t, Selection_operation o)
{
	tag = t;
	op = o;
}

Operation::~Operation()
{
	
}
