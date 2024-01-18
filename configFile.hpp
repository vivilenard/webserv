#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

struct configServer
{
	std::string _serverName;
	std::string _include;
	int			_listen;
	struct Location
	{
		std::string _root;
		std::string _index;
	};
	Location _location;
};

class ConfigFile
{
private:
	std::string _server;
	std::map<std::string, configServer> _configMap;
public:
	ConfigFile();
	std::map<std::string, configServer> readFile(std::string fileName);
	std::map<std::string, configServer> getConFile();
	void								setConFile(std::map<std::string, configServer> set);
	~ConfigFile();
};