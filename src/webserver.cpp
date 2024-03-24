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

static uint32_t	extractPort(struct sockaddr* address) {
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
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "SERVERNAME: " + server._serverName << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "INCLUDE: " + server._include << FPurple << "║" << std::endl;
	for (std::vector<struct socketParameter>::iterator it = server._socketAddress.begin(); it != server._socketAddress.end(); it++) {
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ADDRESS: " + std::string(inet_ntoa(((struct sockaddr_in*)it->interfaceAddress)->sin_addr)) << FPurple << "║" << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "PORT: " + std::to_string(extractPort(it->interfaceAddress)) << FPurple << "║" << std::endl;
		std::cout << FPurple << "║" << FYellow << "SSL: " << std::setw(49) << std::left << (it->ssl ? "true" : "false") << FPurple << "║" << std::endl;
	}
	// std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "PORT: " + server._portStr << FPurple << "║" << std::endl;
	// std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ADDRESS: " + server._address << FPurple << "║" << std::endl;
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
		http* newExecuter = new http(it->second);
		for (std::vector<struct socketParameter>::iterator it2 = it->second._socketAddress.begin(); it2 != it->second._socketAddress.end(); it2++) {
			try {
				if (it2->ssl) {
					if (!it->second._certificate.empty())
						it2->sslCertificate = it->second._certificate;
					else
						throw std::runtime_error("No certificate specified for ssl");
					if (!it->second._key.empty())
						it2->sslKey = it->second._key;
					else
						throw std::runtime_error("No key specified for ssl");
				}
				Interface::addExecuter(extractPort(it2->interfaceAddress), newExecuter);
				socketManager::addServerSocket(*it2);
			}
			catch (std::exception &e) {
				std::cout << "Exception: " << e.what() << std::endl;
				std::cout << "Skipping socket" << std::endl;
				Interface::removeExecuter(extractPort(it2->interfaceAddress));
			}
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
