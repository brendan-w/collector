

#include <iostream>


#include "display.h"
#include "collector.h"
#include "config.h"
#include "filestore.h"
#include "file.h"



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

	int x = 2;
	int y = 2;
	int inc = 8;

	setRenderDrawColor(renderer, config->get_color(FILL));

	for(File* file: filestore->files)
	{
		file->rect = { x, y, 6, 6 };

		SDL_RenderFillRect(renderer, &(file->rect));

		x += inc;
		if(x > config->window_w)
		{
			x = 2;
			y += inc;
		}
	}
}
