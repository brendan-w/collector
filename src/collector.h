
#pragma once

#include <config.h>
#include <SDL_Context.h>

//the only two main globals
extern SDL_Context* sdl;
extern Config* config;

//each of the custom events (declared in SDL_Context.cpp)
extern Uint32 RENDER;
extern Uint32 SELECTOR;
extern Uint32 SELECTION;
extern Uint32 FILE_INFO;
extern Uint32 TAG_INFO_QUERY;
extern Uint32 TAG_INFO_RESP;

//just to make the code cleaner
#define CLI_H (config->CLI_height)
#define CLI_PAD (config->CLI_padding)
#define FILE_SIZE (config->file_size)
#define FILE_THUMB_SIZE (config->file_thumb_size)
#define FILE_PAD (config->file_padding)
#define FILE_OFFSET (FILE_SIZE + FILE_PAD)
#define FILE_THUMB_OFFSET (FILE_THUMB_SIZE + FILE_PAD)
