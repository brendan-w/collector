
/*
########################################################################
#                                                                      #
# Collector: Tag based file management for Linux                       #
#                                                                      #
# Copyright 2015 Brendan Whitfield (bcw7044@rit.edu)                   #
#                                                                      #
########################################################################
#                                                                      #
# utils.cpp                                                            #
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


#include <iostream>
#include <string>
#include <sstream> //std::stringstream
#include <fstream>
#include <iomanip> //std::fixed, std::setprecision
#include <vector>
#include <algorithm> //std::min()
#include <cmath>
#include <sys/stat.h> //stat()

#include <SDL.h>

#include <filestore/file.h>
#include <utils.h>



void to_lower(std::string & s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

bool file_exists(const char* filename)
{
	std::ifstream fin(filename);
	return fin;
}

bool dir_exists(const char* path)
{
	struct stat st;
	if(stat(path, &st) == 0)
		if((st.st_mode & S_IFDIR) != 0)
			return true;
	return false;
}

bool starts_with(std::string str, std::string partial)
{
	size_t l = partial.length();
	if(l > str.length()) return false;
	return (str.substr(0, l) == partial);
}

bool is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}




std::vector<std::string> split(std::string & str, std::string delims)
{
	std::vector<std::string> parts;

	size_t prev = 0;
	size_t pos = 0;

	//while there is another delimeter
	while((pos = str.find_first_of(delims, prev)) != std::string::npos)
	{
		if(pos > prev)
			parts.push_back(str.substr(prev, pos-prev));

		prev = pos + 1;
	}

	//add the last tag to the set
	if(prev < str.length())
		parts.push_back(str.substr(prev, std::string::npos));

	return parts;
}

Path_Parts get_path_parts(std::string path)
{
	//split the filepath into directories and file name
	Path_Parts p;
	p.dirs = "";
	p.name = path;
	p.ext = "";

	size_t last_dir = p.name.rfind(PATH_SEP);

	if(last_dir != std::string::npos)
	{
		last_dir++; //include the PATH_SEP in the dirs portion, and not the name
		p.dirs = p.name.substr(0, last_dir);
		p.name = p.name.substr(last_dir);
	}

	size_t ext_pos = p.name.rfind(".");

	if(ext_pos != std::string::npos)
	{
		p.ext  = p.name.substr(ext_pos);
		p.name = p.name.substr(0, ext_pos);
	}

	return p;
}

std::string join_path_parts(Path_Parts p)
{
	return path_join(p.dirs, p.name + p.ext);
}

std::string escape_file_name(std::string &path)
{
	std::string output = "";

	for(char &c: path)
	{
		switch(c)
		{
			case ' ':
			case '&':
			case ';':
			case '%':
			case '(':
			case ')':
				output.push_back('\\');
				break;
		}

		output.push_back(c);
	}

	return output;
}

std::string double_to_str(double d, int p)
{
	std::stringstream out;
	out << std::fixed << std::setprecision(p) << d;
	return out.str();
}

std::string pretty_print_file_size(size_t bytes)
{
	//find out what power number we're dealing with
	int p = (int) floor(log10(bytes));

	//cap at 12, since terra is the biggest this supports
	p = std::min(p, 12);

	double size = bytes / pow(10, (double) p);

	std::string result = double_to_str(size, 2);

	if(p < 3)                     result += " b";
	else if((p >= 3) && (p < 6))  result += " Kb";
	else if((p >= 6) && (p < 9))  result += " Mb";
	else if((p >= 9) && (p < 12)) result += " Gb";
	else if(p >= 12)              result += " Tb";

	return result;
}

std::string path_join(std::string a_str, std::string b_str)
{
    bool a = (a_str.back() == PATH_SEP);
    bool b = (b_str.front() == PATH_SEP);

    if(a && b)
        a_str.pop_back();
    else if(!a && !b)
        a_str.push_back(PATH_SEP);

    return a_str + b_str;
}
