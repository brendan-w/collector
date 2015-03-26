


#include <filestore/file.h>
#include <events/selection.h>


Selection::Selection() : Event(SELECTION)
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
