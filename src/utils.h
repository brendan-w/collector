
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
std::string double_to_str(double d, int p);
std::string pretty_print_file_size(size_t size);
std::string path_join(std::string a_str, std::string &b_str);

SDL_Point hilbert_d_to_point(int n, int d);
size_t hilbert_point_to_d(int n, SDL_Point p);

void set_intersect(file_set & out, const file_set & in_A, const file_set & in_B);
void set_union(file_set & out, const file_set & in_A, const file_set & in_B);
