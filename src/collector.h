
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# collector.h                                                          #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/

#pragma once

#include <config.h>
#include <SDL_Context.h>

//the only two main globals
extern SDL_Context* sdl;
extern Config* config;

//each of the custom events (declared in SDL_Context.cpp)
extern Uint32 SELECTOR;
extern Uint32 OPERATION;
extern Uint32 RENDER_THUMBS;
extern Uint32 STATE_CHANGE;

//just to make the code cleaner
#define CLI_H (config->CLI_height)
#define CLI_PAD (config->CLI_padding)
#define FILE_SIZE (config->file_size)
#define FILE_THUMB_SIZE (config->file_thumb_size)
#define FILE_PAD (config->file_padding)
#define FILE_OFFSET (FILE_SIZE + FILE_PAD)
#define FILE_THUMB_OFFSET (FILE_THUMB_SIZE + FILE_PAD)
