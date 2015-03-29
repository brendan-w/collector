
#include <string.h>

#include <SDL.h>

#include <utils.h>
#include <config.h>


#ifdef _WIN32
	#include <direct.h>
	#define getcwd _getcwd
#else
	#include <unistd.h> //getcwd()
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
	// cwd_path = "/home/brendan/Andromeda";
	cwd_path = "/home/brendan/cool";

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

	//the delimeters by which paths will be split
	tag_delim = " ._-+&%%()[]{}";
	tag_delim += PATH_SEP;
	font_path = path_join(bin_path, "../assets/MonoLiberation.ttf");
	
	fullscreen = false;
	resizable = true;
	window = {
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480
	};

	colors[BACKGROUND]    = { 50,  50,  50,  255 };
	colors[FILL]          = { 75,  75,  75,  255 };
	colors[HIGHLIGHT]     = { 48,  141, 186, 255 };
	colors[OVERLAY]       = { 0,   0,   0,   140 };
	colors[CLI_HIGHLIGHT] = { 48,  141, 186, 110 };
	colors[CLI_TEXT]      = { 255, 255, 255, 255 };
	
	font_size = 12;
	max_suggest = 4;
	scroll_speed = 60;
	file_size = 15;
	file_thumb_size = 99;
	file_padding = 1;
	CLI_height = 20;
	CLI_padding = 4;
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

SDL_Rect Config::get_window_rect()
{
	return {
		0, //disregard location on screen
		0,
		window.w,
		window.h
	};
}
