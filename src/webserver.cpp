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

void printConfig(configServer server, int index) {
	std::cout << FPurple << "╔══════════════════════════════════════════════════════╗" << NORMAL << std::endl;
	std::stringstream nameStream;
	nameStream << Cyan << "SERVER: " << FCyan << index << "    " << FGreen << (server.validFormat ? "[VALID]" : "") << FRed << (server.validFormat ? "" : "[INVALID]");
	std::cout << FPurple << "║" << std::setw(82) << std::left << nameStream.str() << FPurple << "║" << NORMAL << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "SERVERNAME: " + server._serverName << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "INCLUDE: " + server._include << FPurple << "║" << std::endl;
    for (std::vector<struct socketParameter>::iterator it = server._socketAddress.begin(); it != server._socketAddress.end(); it++) {
		std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
        std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ADDRESS: " + std::string(inet_ntoa(((struct sockaddr_in*)it->interfaceAddress)->sin_addr)) << FPurple << "║" << std::endl;
        std::stringstream portStream;
        portStream << extractPort(it->interfaceAddress);
        std::string portStr = portStream.str();

        std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "PORT: " + portStr << FPurple << "║" << std::endl;
		std::stringstream sslStream;
		sslStream << FYellow << "SSL: " << Green << (it->ssl ? "true" : "") << Red << (it->ssl ? "" : "false");
        std::cout << FPurple << "║" << std::setw(75) << std::left << sslStream.str() << FPurple << "║" << std::endl;
        if (it->ssl && (server._certificate.empty() || server._key.empty())) {
			std::cout << FPurple << "║" << FYellow << std::setw(61) << std::left << "Socket \033[1;31m[INVALID]" << FPurple << "║" << std::endl;
			std::cout << FPurple << "║" << FRed << std::setw(54) << std::left << "NO CERTIFICATE OR KEY SPECIFIED" << FPurple << "║" << std::endl;
		}
		else {
			std::cout << FPurple << "║" << FYellow << std::setw(61) << std::left << "Socket \033[1;32m[VALID]" << FPurple << "║" << std::endl;
		}
    }
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "ROOT: " + server._root << FPurple << "║" << std::endl;
	std::stringstream	formatStream;
	formatStream << FYellow << "VALIDFORMAT: " << FGreen << ((server.validFormat) ? "true" : "") << FRed << ((server.validFormat) ? "" : "false");
	std::cout << FPurple << "║" << std::setw(75) << std::left << formatStream.str() << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "INDEX: " + server._index << FPurple << "║" << std::endl;
	std::cout << FPurple << "║" << FYellow << std::left << "DIRECTORYLISTING: " << std::setw(36) << std::left << (server._directoryListing ? "true" : "false") << FPurple << "║" << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::cout << FPurple << "║" << FCyan << std::setw(54) << std::left << "LOCATIONS" << FPurple << "║" << NORMAL << std::endl;
	std::cout << FPurple << "╠══════════════════════════════════════════════════════╣" << NORMAL << std::endl;
	std::map<std::string, configServer::Location>::iterator it = server._locations.begin();
	for (; it != server._locations.end(); it++) {
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Location: "  + it->first << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Name: " + it->second._name << FPurple << "║" << NORMAL << std::endl;
		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "Default page: " + it->second._index << FPurple << "║" << NORMAL << std::endl;



		std::stringstream postStream;
		postStream << FYellow << "Post: " << Green << (it->second._post ? "true" : "") << Red << (it->second._post ? "" : "false");
		std::cout << FPurple << "║" << FYellow << std::setw(75) << std::left << postStream.str() << FPurple << "║" << NORMAL << std::endl;


		std::stringstream	getStream;
		getStream << FYellow << "Get: " << Green << (it->second._get ? "true" : "") << Red << (it->second._get ? "" : "false");
		std::cout << FPurple << "║" << FYellow << std::setw(75) << std::left << getStream.str() << FPurple << "║" << NORMAL << std::endl;


		std::stringstream	deleteStream;
		deleteStream << FYellow << "Delete: " << Green << (it->second._delete ? "true" : "") << Red << (it->second._delete ? "" : "false");
		std::cout << FPurple << "║" << FYellow << std::setw(75) << std::left << deleteStream.str() << FPurple << "║" << NORMAL << std::endl;


		std::cout << FPurple << "║" << FYellow << std::setw(54) << std::left << "redirection: " + it->second._redirect << FPurple << "║" << NORMAL << std::endl;
	}
	std::cout << FPurple << "╚══════════════════════════════════════════════════════╝" << NORMAL << std::endl;
}

int main(int argc, char **argv)
{
	signal(SIGINT, &sigHandler);
	if (!readConfig(argc, argv, config))
		return 1;
	int index = 1;
	for (CONFIG::iterator it2 = config.begin(); it2 != config.end(); it2++) {
		printConfig(it2->second, index);
		index++;
	}
	for (CONFIG::iterator it = config.begin(); it != config.end(); it++) {
		http* newExecuter = new http(it->second);
		uint32_t tmpPort;
		bool	validSocket = false;
		for (std::vector<struct socketParameter>::iterator it2 = it->second._socketAddress.begin(); it2 != it->second._socketAddress.end(); it2++) {
			tmpPort = extractPort(it2->interfaceAddress);
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
				Interface::addExecuter(tmpPort, newExecuter);
				socketManager::addServerSocket(*it2);
				validSocket = true;
			}
			catch (std::exception &e) {
				PRINT_ERROR;
				std::cout << "Skipping socket" << std::endl;
				if (!validSocket && it2 == it->second._socketAddress.end())
					Interface::removeExecuter(tmpPort);
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
                PRINT_ERROR;
                std::cout << "Exiting program" << std::endl;
                socketManager::stop();
				Interface::clearExecuters();
				return 1;
            }
			PRINT_ERROR;
			std::cout << "Restarting server" << std::endl;
		}
	}
	return 2;
}
