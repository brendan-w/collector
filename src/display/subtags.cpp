
#include <iostream>
#include <string>

#include <SDL.h>

#include <collector.h>
#include <text.h>
#include <filestore/selection.h>
#include <display/subtags.h>


Subtags::Subtags(Selection** s) : DisplayObject(s)
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

	tag_vector subtags = selection()->get_subtags();

	for(std::string tag: subtags)
	{
		Text* text = new Text(tag, config->get_color(CLI_DARK));
		tags.push_back(text);
	}

	mark_dirty();
}
