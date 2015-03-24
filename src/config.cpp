
#include <string.h>

#include <SDL.h>
#include "config.h"


#ifdef _WIN32
	#include <direct.h>
	#define getcwd _getcwd
	const char PATH_SEP = '\\';
#else
	#include <unistd.h> //getcwd()
	const char PATH_SEP = '/';
#endif


Config::Config()
{
	/*
		Set default configuration before reading config file
	*/

	//use the executable location to find the assets
	bin_path = std::string(SDL_GetBasePath());

	//get the pathname for the current working directory
	// cwd_path = std::string(getcwd(NULL, 0));
	// cwd_path = "~/Andromeda/";
	cwd_path = "~/cool/Lasers/";

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

	tag_delim = "._-+ ";
	tag_delim += PATH_SEP;
	font_path = bin_path + "../assets/MonoLiberation.ttf";
	
	fullscreen = false;
	resizable = true;
	window_x = SDL_WINDOWPOS_UNDEFINED;
	window_y = SDL_WINDOWPOS_UNDEFINED;
	window_w = 640;
	window_h = 480;

	colors[BACKGROUND] = { 0,   0,   0,   255};
	colors[FILL]       = { 50,  50,  50,  255};
	colors[HIGHLIGHT]  = { 18,  53,  70,  255};
	colors[CLI_TEXT]   = { 255, 255, 255, 255};
	
	font_size = 12;
	max_suggest = 4;
	scroll_speed = 20;
	file_size = 5;
}

Config::~Config()
{

}

/*
	Getters
*/

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

SDL_Color Config::get_color(Color c) { return colors[c]; }
