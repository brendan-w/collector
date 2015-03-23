

#include <string>
#include <vector>
#include <algorithm>

#include "utils.h"


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
