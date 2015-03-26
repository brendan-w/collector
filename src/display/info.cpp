
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <string>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <text.h>
#include <filestore/file.h>
#include <display/info.h>


Info::Info(Selection** s) : DisplayObject(s)
{
	info = new Text("", config->get_color(CLI_TEXT));
}


Info::~Info()
{
	delete info;
}

void Info::render()
{
	//draw the background
	setRenderDrawColor(renderer, config->get_color(OVERLAY));
	SDL_RenderFillRect(renderer, &rect);

	info->render(0,
				   rect.y + config->CLI_padding);
}


void Info::layout()
{
	rect = {
		0,
		0,
		config->window.w,
		config->CLI_height
	};
}

void Info::on_file_info(File* f)
{
	file = f;

	if(file == NULL)
	{
		info->set_text("");
	}
	else
	{
		info->set_text(file->get_path());
	}
}
