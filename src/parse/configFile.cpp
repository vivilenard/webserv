#include "configFile.hpp"

ConfigFile::ConfigFile() {}

void ConfigFile::readFile(std::string &fileName, std::map<std::string, configServer> & config)
{
	std::ifstream inputFile(fileName.c_str());
	configServer tmpServer;
	int size = 0;
	tmpServer = initializeObj();
	this->_mimeTypes = parseMime();
	if (_mimeTypes.empty())
	   std::cout << "no mimeTypes included. Save them in this directory: /config/mime.types" << endl;
	tmpServer._mimeTypes = this->_mimeTypes;
	if (inputFile.is_open())
	{
		std::string line;
		while (getline(inputFile, line))
		{
			std::string token;
			std::istringstream find(line);
			find >> token;
			if (token == "server")
			{
				size++;
				if (token == "server" && size > 1)
				{
					// if (!checkSSLParameter(tmpServer))
						// std::cout << "SSL parameters missing, skipping socket" << std::endl;
					// else
						config.insert(std::pair<std::string, configServer>(tmpServer._serverName, tmpServer));
					tmpServer = initializeObj();
					tmpServer._mimeTypes = this->_mimeTypes;
				}
			}
			if (!token.empty())
			{
				addServerName(tmpServer, token, find);
				addAddress(tmpServer, token, find);
				addCertificate(tmpServer, token, find);
				addKey(tmpServer, token, find);
				addRoot(tmpServer, token, find);
				addIndex(tmpServer, token, find);
				setLocation(tmpServer, inputFile, token, line, find);
				if (!tmpServer.validFormat)
				{
					std::cout << "THE ERROR---> " << token << std::endl;
					break;
				}
			}
		}
		// checkSSLParameter(tmpServer);
		config.insert(std::pair<std::string, configServer>(tmpServer._serverName, tmpServer));
	}
	else
	   std::cout << "wrong format" << std::endl;
	inputFile.close();
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

void	ConfigFile::addAddress(configServer &server, std::string token, std::istringstream &find)
{
	std::string				tmp;
	std::istringstream		portStr;

	if (token == "listen")
	{
		if (!find.good()) {
			std::cerr << "ConfigFile::addAddress:	critical input error" << std::endl;
			exit(1);
		}
		if (find >> tmp) {
			if (tmp[0] == '[') {
				size_t pos = tmp.find("]");
				server._address = tmp.substr(1, pos - 1);
				portStr.str(tmp.substr(pos + 2));
				server._portStr = tmp.substr(pos + 2);
				portStr >> server._port;
			}
			else {
				size_t pos = tmp.find(":");
				server._address = tmp.substr(0, pos);
				portStr.str(tmp.substr(pos + 1));
				server._portStr = tmp.substr(pos + 1);
				portStr >> server._port;
			}
			struct socketParameter newSocket;
			try {
				newSocket.interfaceAddress = convertToSockAddr(server._address, server._port);
			}
			catch (std::exception &e) {
				std::cout << "Skipping this socket because of: " << std::endl;
				std::cout << "Exception: " << e.what() << std::endl;
				
			}
			newSocket.protocol = TCP;
			if (find >> tmp && tmp == "ssl")
				newSocket.ssl = true;
			else
				newSocket.ssl = false;
			server._socketAddress.push_back(newSocket);
		}
		else
			server.validFormat = false;
	}
}

void	ConfigFile::addCertificate(configServer &server, std::string token, std::istringstream &find)
{
	std::string certificate;
	if (token == "ssl_certificate")
	{
		if (find >> certificate)
			server._certificate = certificate;
		else
		{
			server.validFormat = false;
			throw std::runtime_error("ConfigFile::addCertificate: No path for the certificate specified");
		}
	}
}

void	ConfigFile::addKey(configServer &server, std::string token, std::istringstream &find)
{
	std::string key;
	if (token == "ssl_certificate_key")
	{
		if (find >> key)
			server._key = key;
		else
		{
			server.validFormat = false;
			throw std::runtime_error("ConfigFile::addKey: No path for the key specified");
		}
	}
}


void	ConfigFile::addIndex(configServer &server, std::string token, std::istringstream &find)
{
	std::string index;
	if (token == "index")
	{
		if (find >> index)
			server._index = index;
		else
		{
			server.validFormat = false;
			std::cout << "Invalid index exiting ..." << std::endl;
		}
	}
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
