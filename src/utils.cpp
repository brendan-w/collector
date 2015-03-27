

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <SDL.h>

#include <filestore/file.h>
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

std::string int_to_str(int i)
{
    std::stringstream out;
    out << i;
    return out.str();
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

void intersect(file_set & out, const file_set & in_A, const file_set & in_B)
{
    if(in_B.size() < in_A.size())
    {
        //swap the arguments so that we only iterate throught the smaller set
        intersect(out, in_B, in_A);
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
