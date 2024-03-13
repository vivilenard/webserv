#include "configFile.hpp"

ConfigFile::ConfigFile() { cout << "CREATING CONFIG FILE" << endl; }

void	serverStatus(configServer &server)
{
	if (server.validFormat == true)
		std::cout << "Server Status [ONLINE] " << server.validFormat << std::endl;
	else
		std::cout << "Server Status [OFFLINE] " << server.validFormat << std::endl;
}

void	ConfigFile::addListen(configServer &server, std::string token, std::istringstream &find)
{

	if (token == "listen")
		addAddress(server, find);
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

void ConfigFile::readFile(std::string &fileName, std::map<std::string, configServer> & config)
{
 	std::ifstream inputFile(fileName.c_str());
 	configServer tmpServer;
	tmpServer = initializeObj();
	this->_mimeTypes = parseMime();
	if (_mimeTypes.empty())
		std::cout << "no mimeTypes included. Save them in this directory: /config/mime.types" << endl;
// printMimes(this->_mimeTypes);
	tmpServer._mimeTypes = this->_mimeTypes;
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
			if (!tmpServer.validFormat )
				break ;
			addRoot(tmpServer, token, find);
			addIndex(tmpServer, token, find);
			setLocation(tmpServer,inputFile, token,line, find);
		}
		config[tmpServer._serverName] = tmpServer;
		serverStatus(tmpServer);
	}
	else
 		std::cout << "wrong format" << std::endl;
	inputFile.close();
}

void	ConfigFile::setConFile(std::map<std::string, configServer> &set)
{
	this->_configMap = set;
}

std::map<std::string, configServer> ConfigFile::getConFile()
{
	return (this->_configMap);
}

ConfigFile::~ConfigFile() {}
