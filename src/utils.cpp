

#include <string>
#include <vector>

#include "utils.h"


unsigned int levenshtein_distance(const std::string & s1, const std::string & s2) {
	const size_t len1 = s1.size(), len2 = s2.size();
	std::vector<unsigned int> col(len2+1), prevCol(len2+1);

	for(unsigned int i = 0; i < prevCol.size(); i++)
	{
		prevCol[i] = i;
	}

	for(unsigned int i = 0; i < len1; i++)
	{
		col[0] = i+1;
		for (unsigned int j = 0; j < len2; j++)
		{
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1),
								prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}
