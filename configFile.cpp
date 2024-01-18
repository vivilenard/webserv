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
		std::string serverName;
		int port = 0;
		std::istringstream find(line);
		find >> token;
		if (token == "listen")
		{
			if (find >> port)
				tmpServer._listen = port;
			else
				std::cout << "No port" << std::endl;
		}
		else if (token == "server_name")
		{
			if (find >> serverName)
				tmpServer._serverName = serverName.substr(0, serverName.length() - 1); // bc each line ends with ";"
			// ADD A MSG IF THERE IS NOT A SEMICOLON IN RED BIG!!
			else
				std::cout << "invalid format" << std::endl;
		}
		else if (token == "location")
		{
			std::string root;
			if (find >> root)
			{
				tmpServer._location._root = root;
				std::cout << root << std::endl;
			}
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