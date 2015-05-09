
#include <string>
#include <vector>

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

}

void Subtags::render()
{
	//draw the background
	SDL_Rect rect = sdl->get_viewport();
	sdl->set_color(OVERLAY);
	sdl->fill_rect(rect);

	render_tags();
}

void Subtags::render_tags()
{
	SDL_Rect rect = sdl->get_viewport();
	SDL_Rect tag_rect = rect;

	//draw each tags text
	// sdl->set_color(CLI_HIGHLIGHT);
	sdl->set_color(OVERLAY);

	int x = CLI_PAD;
	for(unsigned int i = 0; i < tags.size(); i++)
	{
		Text* t = tags[i];
		
		if(i == current_index)
		{
			tag_rect.x = x - CLI_PAD;
			tag_rect.w = t->width() + (CLI_PAD * 2);
			sdl->fill_rect(tag_rect);
		}

		t->render(x, rect.y + CLI_PAD);
		x += t->width() + (CLI_PAD * 2);
	}
}

void Subtags::on_selection()
{
	Selection* s = selection();

	//update the internal state
	std::string str = "";
	str += std::to_string(s->size());
	str += " / ";
	str += std::to_string(s->all_size());

	totals->set_text(str);
}
