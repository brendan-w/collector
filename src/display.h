
#pragma once

#include "filestore.h"


class Display
{
	public:
		Display();
		~Display();
		void handleEvent();
		void render();

	private:
		FileStore filestore;
};
