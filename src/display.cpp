

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

	int x = 1;
	int y = 1;
	int inc = 6;

	setRenderDrawColor(renderer, config->get_color(FILL));

	for(File* file: filestore->files)
	{
		file->rect = { x, y, 5, 5 };

		SDL_RenderFillRect(renderer, &(file->rect));

		x += inc;
		if(x > config->window_w)
		{
			x = 1;
			y += inc;
		}
	}
}
