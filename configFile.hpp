#pragma once

#include <string>
#include <fstream>
#include <iostream>

struct configServer
{
	std::string _serverName;
	std::string _listen;
	std::string _include;
	std::string _index;
	int port;

};

class ConfigFile
{
private:
	std::string _server;
public:
	ConfigFile();
	void	readFile();
	~ConfigFile();
};