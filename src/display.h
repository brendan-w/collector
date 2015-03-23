
#pragma once

#include "filestore.h"


class Display
{
	public:
		Display();
		~Display();
		void handleEvent();
		void render();
		void cli_change(void* e_data);

	private:
		FileStore* filestore;
};
