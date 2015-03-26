
#pragma once


#define NUM_USER_EVENTS 2


//SDL userevent types
extern Uint32 SELECTOR;
extern Uint32 SELECTION;


//used by collector.cpp to initialize some SDL events
bool init_events();


class Event
{
	public:
		Event(Uint32 type);
		virtual ~Event() {}
		void submit();
	private:
		Uint32 user_type = 0;
};
