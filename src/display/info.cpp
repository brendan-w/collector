
/*
	The CLI class is a frontend and factory for Selector objects,
	which are used to query the filestore.
*/

#include <string>

#include <SDL.h>

#include <collector.h>
#include <config.h>
#include <utils.h>
#include <text.h>
#include <filestore/file.h>
#include <display/info.h>


Info::Info(Selection** s) : DisplayObject(s)
{
	filepath = new Text("", config->get_color(CLI_TEXT));
	filesize = new Text("", config->get_color(CLI_TEXT));
}


Info::~Info()
{
	delete filepath;
	delete filesize;
}

void Info::render()
{
	//draw the background
	context->set_color(OVERLAY);
	context->fill_rect(rect);

	filepath->render(CLI_PAD,
	                 rect.y + CLI_PAD);

	filesize->render(rect.w - filesize->width() - CLI_PAD,
	                 rect.y + CLI_PAD);
}


void Info::layout(bool force)
{
	rect = {
		0,
		0,
		WINDOW_W,
		CLI_H
	};
}

void Info::on_file_info(File* f)
{
	file = f;

	if(file == NULL)
	{
		filepath->set_text("");
		filesize->set_text("");
	}
	else
	{
		filepath->set_text(file->get_path());
		filesize->set_text(pretty_print_file_size(file->get_size()));
	}
}
