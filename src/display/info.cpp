
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# info.cpp                                                             #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/


#include <string>

#include <SDL.h>

#include <collector.h>
#include <utils.h> //pretty_print_file_size()
#include <text.h>
#include <filestore/file.h>
#include <display/state.h>
#include <display/info.h>


Info::Info(State* s) : DisplayObject(s)
{
	filepath = new Text("", config->get_color(CLI_LIGHT));
	filesize = new Text("", config->get_color(CLI_LIGHT));
}

Info::~Info()
{
	delete filepath;
	delete filesize;
}

void Info::render()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw the background
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	filepath->render(CLI_PAD,
	                 rect.y + CLI_PAD);

	filesize->render(rect.w - filesize->width() - CLI_PAD,
	                 rect.y + CLI_PAD);
}

void Info::on_state_change()
{
	File* file = state->file_under_mouse;

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

	mark_dirty();
}
