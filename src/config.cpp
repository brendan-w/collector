
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
	find_cmd = "find " + cwd_path + " -type f -path \"*\" ! -path \"*/.*\" ! -perm -o=x";

	//command returning the mime-type of the requested file
	mime_cmd = "xdg-mime query filetype ";

	//command to open arbitrary files
	open_cmd = "exo-open ";

	//the delimeters by which paths will be split
	tag_delim = " ._-+&%%()[]{}";
	tag_delim += PATH_SEP;
	font_path = path_join(bin_path, "../assets/MonoLiberation.ttf");
	
	fullscreen = false;
	resizable = true;
	window = {
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1000,
		650
	};

	colors[BACKGROUND]    = { 50,  50,  50,  255 };
	colors[OVERLAY]       = { 0,   0,   0,   255 };
	colors[CLI_HIGHLIGHT] = { 48,  141, 186, 110 };
	colors[CLI_TEXT]      = { 255, 255, 255, 255 };

	colors[FILE_NEUTRAL]        = { 75,  75,  75,  255 };
	colors[FILE_SELECTED]       = { 48,  141, 186, 255 };
	colors[FILE_NEUTRAL_HOVER]  = { 95,  95,  95,  255 };
	colors[FILE_SELECTED_HOVER] = { 68,  161, 206, 255 };

	font_size = 12;
	max_suggest = 4;
	scroll_speed = 120;
	file_size = 9;
	file_thumb_size = 199;
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
