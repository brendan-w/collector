
#pragma once

#include <string>

#include <SDL.h>

SDL_Surface* CreateSurface(int w, int h);
void print_message(std::string message);
void print_SDL_error(std::string message);
void print_TTF_error(std::string message);
void print_IMG_error(std::string message);
