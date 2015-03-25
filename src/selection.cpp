


#include "file.h"
#include "selection.h"


Selection::Selection()
{

}

Selection::~Selection()
{

}

void Selection::add_file(File* file)
{
	if(files.find(file) == files.end())
		files.insert(file);
}
