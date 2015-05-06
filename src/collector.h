
#pragma once

#include <config.h>
#include <SDL_context.h>

//the only two main globals
extern SDL_context* sdl;
extern Config* config;

//each of the custom events
extern Uint32 RENDER;
extern Uint32 SELECTOR;
extern Uint32 SELECTION;
extern Uint32 FILE_INFO;

//just to make the code cleaner
#define WINDOW_W (config->window.w)
#define WINDOW_H (config->window.h)
#define CLI_H (config->CLI_height)
#define CLI_PAD (config->CLI_padding)
#define FILE_SIZE (config->file_size)
#define FILE_THUMB_SIZE (config->file_thumb_size)
#define FILE_PAD (config->file_padding)
#define FILE_OFFSET (FILE_SIZE + FILE_PAD)
#define FILE_THUMB_OFFSET (FILE_THUMB_SIZE + FILE_PAD)
