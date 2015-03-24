
#pragma once

#include <string>


size_t levenshtein_distance(const std::string &s1, const std::string &s2);
void to_lower(std::string & s);
int xy2d (int n, int x, int y);
void d2xy(int n, int d, int *x, int *y);
