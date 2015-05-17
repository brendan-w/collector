
#pragma once

#include <string>



enum Selection_operation
{
	ADD_TAG = 0,
	REMOVE_TAG,
	DELETE_FILES
};


class Operation
{
	public:
		Operation(std::string t, Selection_operation o);
		~Operation();

		std::string get_tag()        { return tag; }
		Selection_operation get_op() { return op; }

	private:
		std::string tag;
		Selection_operation op;
};
