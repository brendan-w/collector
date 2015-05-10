
#pragma once

#include <string>


class Tag_Info
{
	public:
		Tag_Info(std::string partial);
		~Tag_Info();

		std::string get_partial();
		std::string get_completed();
		void set_completed(std::string complete);

	private:
		std::string partial_tag;
		std::string complete_tag;
};
