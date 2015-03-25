
#pragma once

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

//allow public access to the window and rendering contexts
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;

//SDL userevent types
extern Uint32 SELECTOR;

//SDL utils
void setRenderDrawColor(SDL_Renderer* r, SDL_Color color);
bool rectInWindow(SDL_Rect &rect);
void print_message(std::string message);
void print_SDL_error(std::string message);
void print_TTF_error(std::string message);
void print_IMG_error(std::string message);
