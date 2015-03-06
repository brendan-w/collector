

#include <SDL_ttf.h>

#include "collector.h" //needed for font
#include "texture.h" //needed for Texture::render()
#include "text.h"


Text::Text(std::string t, SDL_Color c)
{
	set_text(t);
	set_color(c);
}

//get/set text
void Text::set_text(std::string s) { text = s; }
std::string Text::get_text()       { return text; }

//get/set color
void Text::set_color(SDL_Color c) { color = c;  }
SDL_Color Text::get_color()       { return color; }


bool Text::load_text()
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	if(surface == NULL)
	{
		print_TTF_error("Failed to render text to surface");
		return false;
	}

	bool success = load_surface(surface);
	SDL_FreeSurface(surface);
	return success;
}


void Text::render(int x, int y)
{
	if(text.length() > 0)
	{
		if(text != current_text)
		{
			load_text();
			current_text = text;
		}

		Texture::render(x, y);
	}
}
