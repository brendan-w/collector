
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# selector.h                                                           #
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

#pragma once

#include <unordered_map>

#include <filestore/file.h>


/*
	Data structure for including or excluding a file
	true  = included
	false = excluded
*/
typedef std::unordered_map<File*, bool> InExclude;


enum Set_Operation
{
	INTERSECTION = 0,
	EXCLUSION,
	UNION
};


class Selector
{
	public:
		Selector();
		~Selector();

		void add_tag(Tag tag, Set_Operation op);
		void add_inexclude(File* f, bool included);

		const TagVector get_tag_intersections() { return tag_intersections; }
		const TagVector get_tag_exclusions()    { return tag_exclusions; }
		const InExclude get_inexclude()         { return inexclude; }

		bool is_empty();

	private:
		TagVector tag_intersections;
		TagVector tag_exclusions;
		InExclude inexclude;
};
