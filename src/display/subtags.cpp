
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# subtags.cpp                                                          #
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

#include <iostream>
#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/selection.h>
#include <display/state.h>
#include <display/subtags.h>


Subtags::Subtags(State* s) : DisplayObject(s)
{

}

Subtags::~Subtags()
{
	destroy_tags();
}

void Subtags::destroy_tags()
{
	for(Text* text: tags)
		delete text;
	tags.clear();
}


void Subtags::render()
{
	//draw the background
	SDL_Rect rect = sdl->get_viewport();
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	//divider line
	sdl->set_color(HIGHLIGHT);
	sdl->draw_line(rect.x, 0, rect.w, 0);

	render_tags();
}

void Subtags::render_tags()
{
	SDL_Rect rect = sdl->get_viewport();

	//draw each tags text
	int x = CLI_PAD;
	for(Text* t: tags)
	{
		t->render(x, rect.y + CLI_PAD);
		x += t->width() + (CLI_PAD * 2);
	}
}

void Subtags::on_selection()
{
	destroy_tags();

	TagVector subtags = selection()->get_subtags();

	for(std::string tag: subtags)
	{
		Text* text = new Text(tag, config->get_color(CLI_DARK));
		tags.push_back(text);
	}

	mark_dirty();
}
