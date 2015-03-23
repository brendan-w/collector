

#include <iostream>
#include <vector>
#include <string>

#include "collector.h"
#include "config.h"
#include "selector.h"
#include "filestore.h"
#include "file.h"
#include "display.h"



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

void Display::new_selector(void* e_data)
{
	Selector* selector = (Selector*) e_data;

	Tag_operations ops = selector->get_operations();

	for(Tag_operation* op: ops)
	{
		std::cout << op->get_tag() << " ";
	}

	std::cout << std::endl;

	delete selector;
}
