

#include <filestore/types.h>
#include <display/state.h>


State::State()
{

}

State::~State()
{
	if(selection != NULL)
		delete selection;

	inexclude.clear();
}

void State::replace_selection(Selection* s)
{
	if(selection != NULL)
		delete selection;
	selection = s;
}

void State::fill_selector(Selector* s)
{
	//dump the include/exclude table into the Selector
	for(auto e: inexclude)
		s->add_inexclude(e.first, e.second);
}

bool State::inexclude_has(File* f)
{
	return (inexclude.find(f) != inexclude.end());
}
