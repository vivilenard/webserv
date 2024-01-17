#pragma once

#include <string>
#include <fstream>
#include <iostream>

class ConfigFile
{
private:
	std::string _server;
public:
	ConfigFile();
	void	readFile();
	~ConfigFile();
};