
#pragma once

#include <string>
#include <vector>
#include "texture.h"


class CLITag
{
	public:
		CLITag();
		~CLITag();
		std::string text;
		void render(int x, int y);
		int width();
	private:
		std::string current_text;
		Texture* texture;

};


class CLI
{
	public:
		CLI();
		~CLI();
		void handle_key(SDL_KeyboardEvent &e);
		void handle_text(SDL_TextInputEvent &e);
		void render();
	private:
		std::vector<CLITag*> tags;
		unsigned int current;

		void destroy_tags();
		CLITag* current_tag();
		void send_quit();
};
