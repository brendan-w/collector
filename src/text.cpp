

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


std::string Text::get_text()
{
	return text;
}

void Text::set_text(std::string s)
{
	text = s;

	//if the texture needs updating
	if(text != current_text)
	{
		load_text();
	}	
}


//get/set color


SDL_Color Text::get_color()
{
	return color;
}

void Text::set_color(SDL_Color c)
{ 
	color = c;
}


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
		//prevent rendering of null strings
		free();
	}

	current_text = text;
}
