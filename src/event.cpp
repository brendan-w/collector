
#include <SDL.h>

#include <collector.h> //print_SDL_error()

#include <event.h>


//each of the custom events
Uint32 SELECTOR = 0;
Uint32 SELECTION = 0;
Uint32 FILE_INFO = 0;



bool init_events()
{
	Uint32 begin = SDL_RegisterEvents(NUM_USER_EVENTS);
	if(begin == ((Uint32) -1 ))
	{
		return false;
	}

	SELECTOR  = begin;
	SELECTION = begin + 1;
	FILE_INFO = begin + 2;

	return true;
}


void submit(Uint32 type, void* data1 /*=NULL*/, void* data2 /*=NULL*/)
{
	SDL_Event e;
	SDL_zero(e);

	e.type = type;
	e.user.data1 = data1;
	e.user.data2 = data2;

	//try to push onto SDL event queue
	int r = SDL_PushEvent(&e);

	if(r < 0)
		print_SDL_error("SDL_PushEvent() failed");
	else if(r == 0)
		print_message("SDL_PushEvent() was filtered");
}
