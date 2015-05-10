

#include <string>
#include <sstream> //std::stringstream
#include <fstream>
#include <iomanip> //std::fixed, std::setprecision
#include <vector>
#include <algorithm> //std::min()
#include <cmath>

#include <SDL.h>

#include <filestore/file.h>
#include <config.h>
#include <utils.h>


//Courtesy of:
//https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B
size_t levenshtein_distance(const std::string & s1, const std::string & s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	std::vector<size_t> col(len2+1), prevCol(len2+1);

	for(size_t i = 0; i < prevCol.size(); i++)
	{
		prevCol[i] = i;
	}

	for(size_t i = 0; i < len1; i++)
	{
		col[0] = i+1;
		for (size_t j = 0; j < len2; j++)
		{
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1),
								prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}

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


/*
    Hilbert curve generators

    ╷ ┌───┐ ┌───┐ ╷
    └─┘ ┌─┘ └─┐ └─┘
    ┌─┐ └─┐ ┌─┘ ┌─┐
    │ └───┘ └───┘ │
    └─┐ ┌─────┐ ┌─┘
    ┌─┘ └─┐ ┌─┘ └─┐
    │ ┌─┐ │ │ ┌─┐ │
    └─┘ └─┘ └─┘ └─┘

    Courtesy of:
    https://en.wikipedia.org/wiki/Hilbert_curve

    n = width of square (must be a power of 2)
    d = distance along curve
    x,y = cell coordinate of point on curve
*/
 
//rotate/flip a quadrant appropriately
static void rot(int n, int *x, int *y, int rx, int ry)
{
    if(ry == 0)
    {
        if(rx == 1)
        {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}

//convert (x,y) to d
static int xy2d (int n, int x, int y)
{
    int rx, ry, s, d=0;
    for(s=n/2; s>0; s/=2)
    {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}
 
//convert d to (x,y)
static void d2xy(int n, int d, int *x, int *y)
{
    int rx, ry, s, t=d;
    *x = *y = 0;
    for(s=1; s<n; s*=2)
    {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

/*
	Custom accessors for Hilbert generators
	Applies transformation for sensical vertical tiling.

		Flips over y = x;

        d (distance along curve) may be greater than a single curve,
        in which case, it will overflow to subsequent, stacked, curves
*/

SDL_Point hilbert_d_to_point(int n, int d)
{
    SDL_Point p;

    //number of cells in one H curve
    int d_per_hilbert = SQUARE(n);

    //offset due to multiple H curves
    int extra_y = n * (d / d_per_hilbert);

    //adjust distance along curve to be in single curve
    d = d % d_per_hilbert;

	d2xy(n, d, &(p.y), &(p.x)); //NOTE: swap of X and Y is intentional

    //account for any curves above
    p.y += extra_y;

    return p;
}

size_t hilbert_point_to_d(int n, SDL_Point p)
{
    //number of cells in one H curve
    int d_per_hilbert = SQUARE(n);

    //find out how many H curves down this point is
    int n_preceeding_curves = p.y / n;

    //adjust depth accordingly
    p.y = p.y % n;

    size_t d = xy2d(n, p.y, p.x); //NOTE: swap of X and Y is intentional
	
    //account for any curves above
    d += n_preceeding_curves * d_per_hilbert;

    return d;
}


/*
    Set Operations
*/

void set_intersect(file_set & out, const file_set & in_A, const file_set & in_B)
{
    if(in_B.size() < in_A.size())
    {
        //swap the arguments so that we only iterate through the smaller set
        set_intersect(out, in_B, in_A);
        return;
    }

    out.clear();

    for(file_set::const_iterator it = in_A.begin(); it != in_A.end(); it++)
    {
        if(in_B.find(*it) != in_B.end())
        {
            out.insert(*it);
        }
    }
}

void set_union(file_set & out, const file_set & in_A, const file_set & in_B)
{
    out.clear();
    out.insert(in_A.begin(), in_A.end());
    out.insert(in_B.begin(), in_B.end());
}
