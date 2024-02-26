#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/stat.h>

#define CONFIG std::map<std::string, configServer>

const int MAX_PORT = 65535;

struct configServer
{
	std::string _serverName;
	std::string _include;
	int			_listen;
	std::string _address;
	std::string _root;
	bool	validFormat;
	std::string _index;
	struct Location //_locations[namelocation].whateverinside
	{
		std::string _name;
		std::string _index;
		bool		_post;
		bool		_get;
		bool		_put;

	};
	std::map<std::string, Location> _locations;
};

class ConfigFile
{
private:
	std::string _server;
	std::map<std::string, configServer> _configMap;
public:
	ConfigFile();
	void	runByDefault();
	std::map<std::string, configServer> readFile(std::string fileName);
	std::map<std::string, configServer> getConFile();
	void								setConFile(std::map<std::string, configServer> set);
	void								addListen(configServer &server, std::string token,
											   						std::istringstream &find);
	void								addServerName(configServer &server, std::string token,
													  				std::istringstream &find);
	void								addAddress(configServer &server, std::istringstream &find);
	void								addRoot(configServer &server, std::string token,
																		std::istringstream &find);
	void								addIndex(configServer &server, std::string token,
												 std::istringstream &find);
	std::string							addNameLocation(configServer &server, std::string token,
													   				std::istringstream &find);
	void								setMethod(configServer &server, std::string dir,
												  std::string rule);
	void								setLocation(configServer &server, std::ifstream &inputFile,
													std::string token, std::string &line, std::istringstream &find);
	~ConfigFile();
};

std::string stringConvert(char *input);
bool readConfig(int argc, char **argv, std::map<std::string, configServer> & config);