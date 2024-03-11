#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <sys/stat.h>
#include <string>
#include "socketManager.hpp"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define CONFIG std::map<std::string, configServer>
#define LOCATION std::map<std::string, configServer::Location>
#define MAP std::map<std::string, std::string>

const int MAX_PORT = 65535;

using namespace std;

struct configServer
{
	std::string 			_serverName;
	std::string 			_include;
	int						_listen;
	struct sockParameter	_socketAddress;
	std::string 			_address;
	std::string 			_root;
	bool					validFormat;
	MAP						_mimeTypes;
	std::string 			_index;
	struct Location 		//_locations[namelocation].whateverinside
	{
		std::string 		_name;
		std::string 		_index;
		bool				_post;
		bool				_get;
		bool				_delete;

	};
	std::map<std::string, Location> _locations;
};

class ConfigFile
{
private:
	std::string _server;
	std::map<std::string, configServer> _configMap;
	MAP	_mimeTypes;
public:
	ConfigFile();
	configServer	initializeObj();
	void	runByDefault();
	void	readFile(std::string &fileName, std::map<std::string, configServer> & config);
	std::map<std::string, configServer> getConFile();
	void								setConFile(std::map<std::string, configServer> &set);
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
	MAP 								parseMime();
	void								createKeypairs(MAP & map, std::string s1, std::string s2);
	sockaddr* 							convertToSockAddr(const std::string& ipAddress, int port);
	~ConfigFile();
};

std::string stringConvert(char *input);
bool readConfig(int argc, char **argv, std::map<std::string, configServer> & config);
void	printMimes(MAP mime);