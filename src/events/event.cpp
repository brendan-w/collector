
#include <iostream>

#include <SDL.h>

#include <collector.h>
#include <events/event.h>


//each 
Uint32 SELECTOR = 0;
Uint32 SELECTION = 0;



bool init_events()
{
	Uint32 begin = SDL_RegisterEvents(NUM_USER_EVENTS);
	if(begin == ((Uint32) -1 ))
	{
		return false;
	}

	SELECTOR  = begin;
	SELECTION = begin + 1;

	return true;
}



Event::Event(Uint32 type)
{
	user_type = type;
}

void Event::submit()
{
	if(user_type != 0)
	{
		SDL_Event e;
	    SDL_zero(e);

		e.type = user_type;
		e.user.data1 = (void*) this;

		//try to push onto SDL event queue
		int r = SDL_PushEvent(&e);

		if(r < 0)
			print_SDL_error("SDL_PushEvent() failed");
		else if(r == 0)
			print_message("SDL_PushEvent() was filtered");
	}
	else
	{
		print_message("Can't submit() event: Invalid user_type");
	}
}
