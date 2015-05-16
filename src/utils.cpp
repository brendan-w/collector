

#include <iostream>
#include <string>
#include <sstream> //std::stringstream
#include <fstream>
#include <iomanip> //std::fixed, std::setprecision
#include <vector>
#include <algorithm> //std::min()
#include <cmath>

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
    bool b = (b_str.back() == PATH_SEP);

    if(a && b)
        a_str.pop_back();
    else if(!a && !b)
        a_str.push_back(PATH_SEP);

    return a_str + b_str;
}
