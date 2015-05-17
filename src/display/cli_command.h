
#pragma once

#include <string>
#include <vector>

#include "SDL.h"

#include <filestore/types.h>
#include <display/displayobject.h>
#include <display/state.h>
#include <text.h>



class CLI_Command : public DisplayObject
{
	public:
		CLI_Command(State* s);
		~CLI_Command();

	private:

};
