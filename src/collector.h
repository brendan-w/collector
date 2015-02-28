
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "config.h"

//allow public access to the window and rendering contexts
extern Config* config;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
