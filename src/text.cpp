

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
void Text::set_text(std::string s)
{
	text = s;
	
	//if the texture needs updating
	if(text != current_text)
	{
		load_text();
	}	
}
std::string Text::get_text()       { return text; }

//get/set color
void Text::set_color(SDL_Color c) { color = c;  }
SDL_Color Text::get_color()       { return color; }


void Text::load_text()
{
	if(text.length() > 0)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

		if(surface == NULL)
		{
			print_TTF_error("Failed to render text to surface");
			return;
		}

		load_surface(surface);
		SDL_FreeSurface(surface);
	}
	else
	{
		free();
	}

	current_text = text;
}


void Text::render(int x, int y)
{
	//will not render a null string since that would make
	// w = 0
	// h = 0
	Texture::render(x, y);
}
