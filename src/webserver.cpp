#include "socketManager.hpp"
#include "Interface.hpp"
#include "../include/httpTransfer/Request.hpp"
#include "../include/httpTransfer/Response.hpp"
#include "parse/configFile.hpp"

CONFIG config;
configServer configfile;

void sigHandler(int signum)
{
	std::cout << "shutting down..." << std::endl;
	socketManager::stop();
	exit(signum);
}

std::string testHttp(const std::string &request/* , Config & config */) {
    std::cout << "testHttp called" << std::endl;
    // if (request.empty()) {
    //     std::cout << "request is empty" << std::endl;
    //     return "";
    // } else {
		// cout << configfile._root << endl;
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request, configfile);
		// cout << httpRequest << endl;
		Response	httpResponse(httpRequest, configfile);
		std::string response = httpResponse.getResponse();
		// cout << BLUE << "RESPONSE:\n" << response << NORM <<endl;
		cout << MAG << "<<------------------------" << NORM << endl;
		return response;
    // }
}

uint32_t	extractPort(struct sockaddr* address) 
{
	if (address->sa_family == AF_INET)
		return (ntohs(((struct sockaddr_in*)address)->sin_port));
	else if (address->sa_family == AF_INET6)
		return (ntohs(((struct sockaddr_in6*)address)->sin6_port));
	else
		throw std::runtime_error("extractPort: unknown address family");
}

void printConfig(string name, configServer server)
{
	cout << RED << "CONFIG: " << name << NORM << endl;
	std::cout << "SERVERNAME:" << " " << server._serverName << "\n";
	std::cout << "INCLUDE:" << " " << server._include << "\n";
	std::cout << "LISTEN:" << " " << server._listen << "\n";
	std::cout << "ADDRESS:" << " " << server._address << "\n";
	std::cout << "ROOT:" << " " << server._root << "\n";
	std::cout << "VALIDFORMAT:" << " " << server.validFormat << "\n";
	std::cout << "INDEX:" << " " << server._index << "\n";

	cout << ORANGE << "LOCATIONS" << NORM << endl;
	std::map<std::string, configServer::Location>::iterator it = server._locations.begin();
	for (; it != server._locations.end(); it++)
	{
		cout << "Location: " << it->first << endl;
		cout << "Name: " << it->second._name << endl;
		cout << "Default page: " << it->second._index << endl;
		cout << "Post: " << it->second._post << endl;
		cout << "Get: " << it->second._get << endl;
		cout << "Put: " << it->second._delete << endl;
	}

}

int main(int argc, char **argv)
{
	signal(SIGINT, &sigHandler);
	if (!readConfig(argc, argv, config))
		return 1;
	configfile = config.begin()->second;
	printConfig(config.begin()->first, configfile);
	cout << configfile._serverName << endl;
	if (configfile._serverName == "default")
	{
		cout << BACK << "Webserver is running with a default config file." << NORM << endl;
		cout << BACK << "Run: ./Webserv ['path to configfile'] to include your own." << NORM << endl;
	}
	// for (CONFIG::iterator it = config.begin(); it != config.end(); it++) {
		protocolFunction testFunction = &testHttp;
		Interface::addProtocol(extractPort(config.begin()->second._socketAddress.interfaceAddress), testFunction);
		// for () {
			socketManager::addServerSocket(config.begin()->second._socketAddress);
		// }
	// }
	InterfaceFunction interfaceFunction = &Interface::interface;
	for (int i = 0; i < 10; i++) {
		try {
			socketManager::start(interfaceFunction);
		}
		catch (std::exception &e) {
			std::cout << "Exception: " << e.what() << std::endl;
			std::cout << "Restarting server" << std::endl;
		}
	}
	return 1;
}
