
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# operation.cpp                                                        #
#                                                                      #
# This file is part of Collector                                       #
#                                                                      #
# Collector is free software: you can redistribute it and/or modify    #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Collector is distributed in the hope that it will be useful,         #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Collector.  If not, see <http://www.gnu.org/licenses/>.   #
#                                                                      #
########################################################################
*/


#include <string>

#include <utils.h> //to_lower()
#include <filestore/operation.h>



Operation::Operation(Selection* s) : selection(s)
{

}

Operation::~Operation()
{
	//Note: do NOT delete the selection object
}

void Operation::add(Selection_Operation o)
{
	switch(o)
	{
		case DELETE_FILES:
			delete_files = true;
			break;
		default:
			break;
	}
}

void Operation::add(Selection_Operation o, std::string t)
{
	to_lower(t);

	switch(o)
	{
		case ADD_TAG:
			add_tags.insert(t);
			break;
		case REMOVE_TAG:
			remove_tags.insert(t);
			break;
		default:
			break;
	}

}

bool Operation::is_changing()
{
	return delete_files ||
	       (add_tags.size() > 0) ||
	       (remove_tags.size() > 0);
}
