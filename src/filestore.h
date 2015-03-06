

#include <string>
#include <vector>
#include <set>
#include "file.h"



class FileStore
{
	public:
		FileStore();
		~FileStore();
	private:
		std::string root;
		std::vector<File*> files;
		std::set<std::string> tags;
		void exec_find(std::vector<std::string> &lines);
};
