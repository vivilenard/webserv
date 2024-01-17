#include "configFile.hpp"

ConfigFile::ConfigFile() {}

void ConfigFile::readFile()
{
 std::ifstream inputFile("example.conf");
 if (inputFile.is_open())
 {
	 std::string line;
	 while (getline(inputFile, line))
	 {
		 size_t find;
		 std::string token;
		 find = line.find("listen");
		 std::string	target = "listen";
		 if (find != std::string::npos)
		 {
			 token = line.substr(find, target.length());
			 std::cout << token << std::endl;
		 }
		 else
			 std::cout << "blait" << std::endl;
	 }

 }
 else
	 std::cout << "wrong format" << std::endl;
}

ConfigFile::~ConfigFile() {}