
#pragma once

#include <string>



enum Selection_Operation
{
	ADD_TAG = 0,
	REMOVE_TAG,
	DELETE_FILES
};


class Operation
{
	public:
		Operation(std::string t, Selection_Operation o);
		~Operation();

		std::string get_tag()        { return tag; }
		Selection_Operation get_op() { return op; }

	private:
		std::string tag;
		Selection_Operation op;
};
