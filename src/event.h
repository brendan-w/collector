
#pragma once


#define NUM_USER_EVENTS 2


//SDL userevent types
extern Uint32 SELECTOR;
extern Uint32 SELECTION;


//used by collector.cpp to initialize some SDL events
bool init_events();

void submit(Uint32 type, void* data1=NULL, void* data2=NULL);
