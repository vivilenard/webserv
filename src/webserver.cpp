#include "socketManager.hpp"
#include "Interface.hpp"
#include "../include/config/Config.hpp"
#include "../include/httpTransfer/Request.hpp"
#include "../include/httpTransfer/Response.hpp"
#include "../parse/configFile.hpp"


std::string testHttp(const std::string &request/* , Config & config */) {
    // std::cout << "testHttp called" << std::endl;
    if (request.empty()) {
        std::cout << "request is empty" << std::endl;
        return "";
    } else {
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request);
		// cout << httpRequest << endl;
		Response	httpResponse(httpRequest);
		std::string response = httpResponse.getResponse();
		// cout << BLUE << "RESPONSE:\n" << response << NORM <<endl;
		cout << MAG << "<<------------------------" << NORM << endl;
		return response;
    }
}

void printConfig(CONFIG config)
{
	cout << "All config data: " << endl;
	for(map<string, configServer>::const_iterator it = config.begin();
		it != config.end(); ++it)
	{
		std::cout << "SERVERNAME:" << " " << it->second._serverName << "\n";
		std::cout << "INCLUDE:" << " " << it->second._include << "\n";
		std::cout << "LISTEN:" << " " << it->second._listen << "\n";
		std::cout << "ADDRESS:" << " " << it->second._address << "\n";
		std::cout << "ROOT:" << " " << it->second._root << "\n";
		std::cout << "VALIDFORMAT:" << " " << it->second.validFormat << "\n";
		std::cout << "INDEX:" << " " << it->second._index << "\n";
	// 	for (std::map<std::string, configServer::Location>::iterator it = config._locations.begin(); it != config._locations.end(); ++it)
	// 	{
    //     std::cout << "Key: " << it->first << std::endl;
    //     std::cout << "Name: " << it->second._name << std::endl;
    //     std::cout << "Index: " << it->second._index << std::endl;
    //     std::cout << "Post: " << it->second._post << std::endl;
    //     std::cout << "Get: " << it->second._get << std::endl;
    //     std::cout << "Put: " << it->second._put << std::endl;
    //     std::cout << std::endl;
    // }
		// std::cout << "LOCATIONS: -->" << it->second._locations["default"] << "\n";
/* 		map<string, configServer>::const_iterator itLoc = config. */
	}
}

int main(int argc, char **argv)
{
	CONFIG config;
	readConfig(argc, argv, config);
	printConfig(config);
	// int numWorker = 4;
	// parsing here
	// add application map to interface before forking the workers
	// applicationInterface::addApplication(80, /*&http::application*/);
	//Config config;
	protocolFunction testFunction = &testHttp;
	Interface::addProtocol("HTTP/1.1", testFunction);
	// add sockets
	{
		socketManager::addSocket("0.0.0.0", 80, IPV4, TCP);
	}
	InterfaceFunction interfaceFunction = &Interface::interface;
	socketManager::start(interfaceFunction);
	return 0;
}
