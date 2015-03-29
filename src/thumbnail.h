
#pragma once

#include <pthread.h>
#include <string>

#include <SDL.h>

#include <texture.h>
#include <text.h>


class Thumbnail
{
	public:
		Thumbnail(std::string path);
		~Thumbnail();
		void render(SDL_Rect* rect);
	private:
		void join(bool force);

		bool thread_running;
		pthread_t thread;
		Texture* texture;
};
