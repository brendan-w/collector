

#include <algorithm> //remove_if()

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

void State::inexclude_file(File* f, bool include)
{
	if(f != NULL)
		inexclude[f] = include;
}

void State::toggle_inexclude(File* f, bool include)
{
	if(f != NULL)
	{
		if(inexclude_has(f))
			inexclude.erase(f);
		else
			inexclude[f] = include;
	}
}

bool State::inexclude_has(File* f)
{
	return (inexclude.find(f) != inexclude.end());
}

void State::clear_includes()
{
	remove_inexclude_by_val(true);
}

void State::clear_excludes()
{
	remove_inexclude_by_val(false);
}

void State::remove_inexclude_by_val(bool val)
{
	for(auto i = inexclude.begin(); i != inexclude.end(); )
	{
		if(i->second == val)
			i = inexclude.erase(i);
		else
			i++;
	}
}
