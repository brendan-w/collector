
#pragma once

#include <string>
#include <vector>


class Selection
{
	public:
		Selection();
		~Selection();
		void add_operation(std::string input);
		const Tag_operations get_operations() { return tags; };
	private:
		Tag_operations tags;
};
