#include "socketManager.hpp"
#include "Interface.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "configFile.hpp"
#include "http.hpp"
#include <iomanip>
#include "color.hpp"

CONFIG config;

void sigHandler(int signum)
{
	std::cout << "shutting down..." << std::endl;
	socketManager::stop();
	Interface::clearExecuters();
	exit(signum);
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

void printConfig(std::string name, configServer server) {
	std::cout << FPurple << "╔══════════════════════════════════════════════════════╗" << NORMAL << std::endl;
	std::cout << FPurple << "║" << GREEN << std::setw(54) << std::left << "SERVER: " + name + " " + (server.validFormat ? "[VALID]" : "[INVALID]")  << FPurple << "║" << NORMAL << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "SERVERNAME" + server._serverName << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "INCLUDE: " + server._include << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "LISTEN: " + server._listen << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ADDRESS: " + server._address << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ROOT: " + server._root << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "VALIDFORMAT: " + server.validFormat << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "INDEX: " + server._index << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::left << "DIRECTORYLISTING: " << std::setw(36) << std::left << (server._directoryListing ? "true" : "false") << FPurple << "║" << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::cout << FPurple << "║" << FCyan << std::setw(54) << std::left << "LOCATIONS" << FPurple << "║" << NORMAL << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::map<std::string, configServer::Location>::iterator it = server._locations.begin();
	for (; it != server._locations.end(); it++) {
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Location: " + it->first << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Name: " + it->second._name << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Default page: " + it->second._index << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Post: " + it->second._post << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Get: " + it->second._get << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Put: " + it->second._delete << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "redirection: " + it->second._redirect << FPurple << "║" << NORMAL << std::endl;
	}
	std::cout << FPurple << "╚══════════════════════════════════════════════════════╝" << NORMAL << std::endl;
}

int main(int argc, char **argv)
{
	signal(SIGINT, &sigHandler);
	if (!readConfig(argc, argv, config))
		return 1;
	for (CONFIG::iterator it2 = config.begin(); it2 != config.end(); it2++) {
			printConfig(it2->first, it2->second);
	}
	for (CONFIG::iterator it = config.begin(); it != config.end(); it++) {
			try {
				Interface::addExecuter(extractPort(it->second._socketAddress.interfaceAddress), new http(it->second));
				socketManager::addServerSocket(it->second._socketAddress);
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
				std::cout << "Skipping server" << std::endl;
			}
	}
	InterfaceFunction interfaceFunction = &Interface::interface;
	for (int i = 0; i < 10; i++) {
		try {
			socketManager::start(interfaceFunction);
		}
		catch (std::exception &e) {
			if (dynamic_cast<std::runtime_error*>(&e)->what() == std::string("socketManager::start:	no sockets to manage")) {
                std::cout << "Exception: " << e.what() << std::endl;
                std::cout << "Exiting program" << std::endl;
                socketManager::stop();
				Interface::clearExecuters();
				return 1;
            }
			std::cout << "Exception: " << e.what() << std::endl;
			std::cout << "Restarting server" << std::endl;
		}
	}
	return 2;
}
