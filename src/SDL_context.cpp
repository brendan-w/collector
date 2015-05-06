

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <SDL_context.h>
#include <SDL_utils.h>
#include <event.h>
#include <config.h>


SDL_context::SDL_context()
{
	/*
		SDL2
	*/

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		print_SDL_error("SDL_Init Error");
		success = false;
		return;
	}

	std::string title = "Collector - " + config->cwd_path;
	window = SDL_CreateWindow(title.c_str(),
                              config->window.x,
                              config->window.y,
                              config->window.w,
                              config->window.h,
                              config->get_window_flags());
	if(window == NULL)
	{
		print_SDL_error("SDL_CreateWindow Error");
		success = false;
		return;
	}

	renderer = SDL_CreateRenderer(window,
                                  -1,
                                  config->get_render_flags());
	if(renderer == NULL)
	{
		print_SDL_error("SDL_CreateRenderer Error");
		success = false;
		return;
	}

	SDL_StartTextInput();

	if(SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0)
	{
		print_SDL_error("SDL_SetRenderDrawBlendMode Error");
		success = false;
		return;
	}

	/*
		SDL2_ttf
	*/

	if(TTF_Init() != 0)
	{
		print_TTF_error("SDL_ttf could not initialize");
		success = false;
		return;
	}

	font = TTF_OpenFont(config->font_path.c_str(),
						config->font_size);
	if(font == NULL)
	{
		print_TTF_error("Failed to load font: " + config->font_path);
		success = false;
		return;
	}

	/*
		SDL2_image
	*/

	int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;

	if((IMG_Init(flags) & flags) != flags)
	{
		print_IMG_error("SDL_image could not initialize");
		success = false;
		return;
	}

	/*
		Event Registration
	*/

	if(!init_events())
	{
		print_message("Failed to register custom events");
		success = false;
		return;
	}
}

SDL_context::~SDL_context()
{
	/*
		SDL2_image
	*/

	IMG_Quit();

	/*
		SDL2_ttf
	*/

	TTF_CloseFont(font);
	TTF_Quit();

	/*
		SDL2
	*/

	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void SDL_context::clear()
{
	SDL_RenderClear(renderer);
}

void SDL_context::present()
{
	SDL_RenderPresent(renderer);
}

void SDL_context::setColor(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
}

void SDL_context::fillRect(SDL_Rect &rect)
{
	SDL_RenderFillRect(renderer, &rect);
}

bool SDL_context::rectInWindow(SDL_Rect &rect)
{
	SDL_Rect screen = config->get_window_rect();
	return SDL_HasIntersection(&screen, &rect);
}
