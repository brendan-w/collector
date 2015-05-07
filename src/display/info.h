
#pragma once

#include <string>

#include "SDL.h"

#include <filestore/file.h>
#include <display/displayobject.h>
#include <text.h>

class Info : public DisplayObject
{
	public:
		Info(Selection** s);
		~Info();

		void render();
		void on_file_info(File* f);

	private:
		File* file;
		Text* filepath;
		Text* filesize;
};
