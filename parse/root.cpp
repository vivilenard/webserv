#include "configFile.hpp"

bool checkRoot(std::string &root)
{
	struct stat info;
	if (stat(root.c_str(), &info) != 0)
	{
		std::cerr << "Invalid root ---> " << root << " <---" << std::endl;
		return (false);
	}
	return (true);
}

void ConfigFile::addRoot(configServer &server, std::string token, std::istringstream &find)
{
	if (token == "root")
	{
		std::string root;
		if (find >> root)
		{
			if (checkRoot(root))
			{
				server._root = root;
				std::cout << "Root stored" << std::endl;
				std::cout << server._root << std::endl;
			}
			else
				server.validFormat = false;
		}
	}
}