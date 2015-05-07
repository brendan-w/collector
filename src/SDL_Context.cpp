

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <collector.h>
#include <SDL_Context.h>
#include <SDL_utils.h>


#define NUM_USER_EVENTS 4

Uint32 RENDER = 0;
Uint32 SELECTOR = 0;
Uint32 SELECTION = 0;
Uint32 FILE_INFO = 0;


SDL_Context::SDL_Context()
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

	Uint32 begin = SDL_RegisterEvents(NUM_USER_EVENTS);
	if(begin == ((Uint32) -1 ))
	{
		print_message("Failed to register custom events");
		success = false;
		return;
	}

	RENDER    = begin;
	SELECTOR  = begin + 1;
	SELECTION = begin + 2;
	FILE_INFO = begin + 3;
}


SDL_Context::~SDL_Context()
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


void SDL_Context::submit(Uint32 type, void* data1 /*=NULL*/, void* data2 /*=NULL*/)
{
	SDL_Event e;
	SDL_zero(e);

	e.type = type;
	e.user.data1 = data1;
	e.user.data2 = data2;

	//try to push onto SDL event queue
	int r = SDL_PushEvent(&e);

	if(r < 0)
		print_SDL_error("SDL_PushEvent() failed");
	else if(r == 0)
		print_message("SDL_PushEvent() was filtered");
}

void SDL_Context::quit()
{
	SDL_Event e;
	e.type = SDL_QUIT;
	SDL_PushEvent(&e);
}

void SDL_Context::clear()
{
	SDL_RenderClear(renderer);
}

void SDL_Context::present()
{
	SDL_RenderPresent(renderer);
}

void SDL_Context::set_color(Color c)
{
	set_color(config->get_color(c));
}

void SDL_Context::set_color(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer,
                           color.r,
                           color.g,
                           color.b,
                           color.a);
}

SDL_Point SDL_Context::window_size()
{
	SDL_Point p;
	SDL_GetWindowSize(window, &p.x, &p.y);
	return p;
}

void SDL_Context::fill_rect(SDL_Rect &rect)
{
	SDL_RenderFillRect(renderer, &rect);
}

void SDL_Context::set_viewport(SDL_Rect &rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void SDL_Context::reset_viewport()
{
	SDL_Point w = window_size();
	SDL_Rect r = { 0, 0, w.x, w.y };
	set_viewport(r);
}

SDL_Rect SDL_Context::get_viewport()
{
	SDL_Rect rect;
	SDL_RenderGetViewport(renderer, &rect);
	rect.x = 0;
	rect.y = 0;
	return rect;
}

bool SDL_Context::rect_in_window(SDL_Rect &rect)
{
	SDL_Point size = window_size();
	SDL_Rect screen = { 0, 0, size.x, size.y };
	return SDL_HasIntersection(&screen, &rect);
}
