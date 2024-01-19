#include "configFile.hpp"

ConfigFile::ConfigFile() {}

bool	ConfigFile::addPort(configServer &server, std::string token, std::istringstream &find)
{
	int port = 0;

	if (token == "listen")
	{
		if (find >> port)
		{
			server._listen = port;
			return (true);
		}
		else
		{
			std::cout << "Invalid port" << std::endl;
			return (false);
		}
	}
	return (true);
}

bool	ConfigFile::addServerName(configServer &server, std::string token, std::istringstream &find)
{
	std::string serverName;

	if (token == "server_name")
	{
		if (find >> serverName)
		{
			server._serverName = serverName.substr(0, serverName.length() - 1); // bc each line ends with ";"
			// ADD A MSG IF THERE IS NOT A SEMICOLON IN RED BIG!!
			return (true);
		}
		else
		{
			std::cout << "invalid format" << std::endl;
			return (false);
		}
	}
	return (true);
}

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
		if (!addPort(tmpServer, token, find))
			exit(1);
		else if (!addServerName(tmpServer, token, find))
			exit(1);
		if (token == "location")
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