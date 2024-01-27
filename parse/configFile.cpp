#include "configFile.hpp"

ConfigFile::ConfigFile() {}

void	ConfigFile::addListen(configServer &server, std::string token, std::istringstream &find)
{

	if (token == "listen")
		addAddress(server, find);
}

bool	ConfigFile::addLocation(configServer &server, std::string token, std::istringstream &find)
{
	if (token == "location")
	{
		std::string root;
		if (find >> root)
		{
			server._location._root = root;
			std::cout << root << std::endl;
			return (true);
		}
		else
			return(false);
	}
	return(true);
}

void	ConfigFile::addServerName(configServer &server, std::string token, std::istringstream &find)
{
	std::string serverName;

	if (token == "server_name")
	{
		if (find >> serverName)
		{
			server._serverName = serverName; // bc each line ends with ";"
			// ADD A MSG IF THERE IS NOT A SEMICOLON IN RED BIG!!
		}
		else
		{
			std::cout << "invalid format" << std::endl;
			server.validFormat = false;
		}
	}
	server.validFormat = true;
}

void	ConfigFile::addIndex(configServer &server, std::string token, std::istringstream &find)
{
	std::string index;
	if (token == "index")
	{
		std::cout << "Hello" << std::endl;
		if (find >> index)
		{
			server._index = index;
			std::cout << "Index stored as " << server._index << std::endl;
		}
		else
		{
			server.validFormat = false;
			std::cout << "Invalid index exiting ..." << std::endl;
		}
	}
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
		addServerName(tmpServer, token, find);
		addListen(tmpServer, token, find);
		addRoot(tmpServer, token, find);
		addIndex(tmpServer, token, find);
		if (token == "location")
			break ;
	 }

 }
 else
	 std::cout << "wrong format" << std::endl;
 configTmp[tmpServer._serverName] = tmpServer;
 if (tmpServer.validFormat == true)
	 std::cout << "Server Status --->  [ONLINE] " << tmpServer.validFormat << std::endl;
 else
	 std::cout << "Server Status --->  [OFFLINE] " << tmpServer.validFormat << std::endl;
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