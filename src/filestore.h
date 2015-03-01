

#include <string>
#include <vector>
#include <set>



class FileStore
{
	public:
		FileStore();
		~FileStore();
	private:
		std::string root;
		std::set<std::string> tags;
		void exec_find(std::string query, std::vector<std::string> &lines);
};
