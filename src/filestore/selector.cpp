
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# selector.cpp                                                         #
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


#include <filestore/file.h>
#include <filestore/selector.h>

/*
	Selector object
*/

Selector::Selector()
{

}

Selector::~Selector()
{
	tag_intersections.clear();
	tag_exclusions.clear();
	inexclude.clear();
}

void Selector::add_tag(Tag tag, Set_Operation op)
{
	if(tag.length() > 0)
	{
		switch(op)
		{
			case INTERSECTION:
				tag_intersections.push_back(tag);
				break;
			case EXCLUSION:
				tag_exclusions.push_back(tag);
				break;
			case UNION:
				break;
		}
	}
}

void Selector::add_inexclude(File* f, bool included)
{
	inexclude[f] = included;
}

bool Selector::is_empty()
{
	return (tag_intersections.size() + tag_exclusions.size() + inexclude.size()) == 0;
}
