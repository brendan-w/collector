
#include "display.h"
#include "collector.h"
#include "filestore.h"



Display::Display()
{
	filestore = new FileStore;
}

Display::~Display()
{
	delete filestore;
}

void Display::render()
{

}
