#include "configFile.hpp"

ConfigFile::ConfigFile() {}

std::map<std::string, configServer> ConfigFile::readFile(std::string fileName)
{
 std::ifstream inputFile(fileName);
 configServer tmpServer;
 std::map<std::string, configServer> configTmp;
 if (inputFile.is_open())
 {
	 std::string line;
	 while (getline(inputFile, line))
	 {
		std::string token;
		std::istringstream find(line);
		find >> token;
		if (token == "listen")
		{
			std::cout << "LISTEN FOUND " << std::endl;
			std::cout << token << std::endl;
			tmpServer._listen = token;
		}
		else if (token == "server_name")
		{
			std::cout << "SERVER NAME " << std::endl;
			tmpServer._serverName = token;
		}
	 }

 }
 else
	 std::cout << "wrong format" << std::endl;
 configTmp[tmpServer._serverName] = tmpServer;
 return (configTmp);
}

void	ConfigFile::setConFile(std::map<std::string, configServer> set)
{
	this->_configMap = set;
}

std::map<std::string, configServer> ConfigFile::getConFile()
{
	return (this->_configMap);
}

ConfigFile::~ConfigFile() {}