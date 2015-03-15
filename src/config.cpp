
#include <string.h>
#include <linux/limits.h> //for PATH_MAX

#include <SDL.h>
#include "config.h"


#ifdef _WIN32
	#include <direct.h>
    #define getcwd _getcwd
#else
	#include <unistd.h>
#endif


Config::Config()
{
	load_defaults();
	load_file();
}

Config::~Config()
{

}

void Config::load_defaults()
{
	//use the executable location to find the assets
	bin_path = std::string(SDL_GetBasePath());

	//get the pathname for the current working directory
	cwd_path = std::string(getcwd(NULL, 0));

	//lists all items in the current directory and below
	//only returns files
	//excludes dot-files and dot-folders
	//excludes executables where o=x
	#ifdef _WIN32
		//untested
		find_cmd = "dir * /b/s";
	#else
		find_cmd = "find " + cwd_path + " -type f -path \"*\" ! -path \"*/.*\" ! -perm -o=x";
	#endif

	tag_delim = "/._-+ ";
	font_path = bin_path + "../assets/MonoLiberation.ttf";
	
	fullscreen = false;
	resizable = true;
	window_x = SDL_WINDOWPOS_UNDEFINED;
	window_y = SDL_WINDOWPOS_UNDEFINED;
	window_w = 640;
	window_h = 480;

	font_size = 12;

	colors[BACKGROUND] = { 0,   0,   0,   255};
	colors[FILL]       = { 40,  40,  40,  255};
	colors[HIGHLIGHT]  = { 18,  53,  70,  255};
	colors[CLI_TEXT]   = { 255, 255, 255, 255};
}

void Config::load_file()
{

}



/*
	Getters
*/

std::string Config::get_bin_path() { return bin_path; }
std::string Config::get_cwd_path() { return cwd_path; }
std::string Config::get_find_cmd() { return find_cmd; }
std::string Config::get_tag_delim() { return tag_delim; }

Uint32 Config::get_window_flags()
{
	Uint32 flags = 0;
	flags |= SDL_WINDOW_SHOWN;
	flags |= (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	flags |= (resizable ? SDL_WINDOW_RESIZABLE : 0);
	return flags;
}

Uint32 Config::get_render_flags()
{
	return SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
}

int Config::get_window_x() { return window_x; }
int Config::get_window_y() { return window_y; }
int Config::get_window_w() { return window_w; }
int Config::get_window_h() { return window_h; }

std::string Config::get_font_path()      { return font_path; }
int Config::get_font_size()              { return font_size; }

SDL_Color Config::get_color(Color c) { return colors[c]; }
