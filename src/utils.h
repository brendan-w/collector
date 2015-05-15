
#pragma once

#include <string>

#include <SDL.h>

#include <filestore/file.h>

#define SQUARE(x) (x*x)

#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

size_t levenshtein_distance(const std::string &s1, const std::string &s2);
void to_lower(std::string & s);
bool file_exists(const char* filename);
bool starts_with(std::string str, std::string partial);
bool is_number(const std::string& s);
std::string double_to_str(double d, int p);
std::string pretty_print_file_size(size_t size);
std::string path_join(std::string a_str, std::string b_str);

SDL_Point hilbert_d_to_point(int n, int d);
size_t hilbert_point_to_d(int n, SDL_Point p);


/*
    Set Operations
*/

template<typename T>
void set_intersect(T & out, const T & in_A, const T & in_B)
{
    if(in_B.size() < in_A.size())
    {
        //swap the arguments so that we only iterate through the smaller set
        set_intersect<T>(out, in_B, in_A);
        return;
    }

    out.clear();

    for(auto it = in_A.begin(); it != in_A.end(); it++)
    {
        if(in_B.find(*it) != in_B.end())
            out.insert(*it);
    }
}

template<typename T>
void set_union(T & out, const T & in)
{
    out.insert(in.begin(), in.end());
}
