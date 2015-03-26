
#pragma once

#include <string>


size_t levenshtein_distance(const std::string &s1, const std::string &s2);
void to_lower(std::string & s);
std::string int_to_str(int i);
void hilbert_d_to_point(int n, int d, int *x, int *y);
int hilbert_point_to_d(int n, int x, int y);
