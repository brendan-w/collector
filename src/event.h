
#pragma once


#define NUM_USER_EVENTS 3


//SDL userevent types
extern Uint32 RENDER; //a dummy event to kick SDL_WaitEvent() for a new render cycle
extern Uint32 SELECTOR;
extern Uint32 SELECTION;
extern Uint32 FILE_INFO;


//used by collector.cpp to initialize some SDL events
bool init_events();

void submit(Uint32 type, void* data1=NULL, void* data2=NULL);
