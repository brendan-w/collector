
#pragma once

#include <string>

#include <SDL.h>

#define SQUARE(x) (x*x)

size_t levenshtein_distance(const std::string &s1, const std::string &s2);
void to_lower(std::string & s);
std::string int_to_str(int i);
SDL_Point hilbert_d_to_point(int n, int d);
size_t hilbert_point_to_d(int n, SDL_Point p);
