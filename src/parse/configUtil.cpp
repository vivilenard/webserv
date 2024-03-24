#include "configFile.hpp"


configServer ConfigFile::initializeObj()
{
	configServer setUp;

	setUp._serverName = "";
	setUp._include = "";
	setUp._port = 0;
	setUp._address = "";
	setUp._root = "";
	setUp.validFormat = false;
	setUp._index = "";
	return (setUp);
}

sockaddr* ConfigFile::convertToSockAddr(const std::string& ipAddress, int port)
{
	if (port < 0 || port > 0xFFFF)
		throw std::runtime_error("ConfigFile::convertToSockAddr: Port out of range");
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status = getaddrinfo(ipAddress.c_str(), NULL, &hints, &res);
	if (status != 0)
		throw std::runtime_error("ConfigFile::convertToSockAddr: Failed to get address info: ");

	struct sockaddr_in* ipv4 = new sockaddr_in;
	struct sockaddr_in6* ipv6 = new sockaddr_in6;
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next) 
	{
		if (p->ai_family == AF_INET) 
		{
			memcpy(ipv4, p->ai_addr, sizeof(sockaddr_in));
			ipv4->sin_port = htons(port);
			freeaddrinfo(res);
			delete ipv6;
			return reinterpret_cast<sockaddr*>(ipv4);
		}
		else if (p->ai_family == AF_INET6) 
		{
			memcpy(ipv6, p->ai_addr, sizeof(sockaddr_in6));
			ipv6->sin6_port = htons(port);
			freeaddrinfo(res);
			delete ipv4;
			return reinterpret_cast<sockaddr*>(ipv6);
		}
	}
	freeaddrinfo(res);
	throw std::runtime_error("ConfigFile::convertToSockAddr: Failed to convert address to sockaddr");
}
