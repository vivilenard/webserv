#include "configFile.hpp"


configServer ConfigFile::initializeObj()
{
	configServer setUp;

	setUp._serverName = "";
	setUp._include = "";
	setUp._listen = 0;
	setUp._address = "";
	setUp._root = "";
	setUp.validFormat = false;
	setUp._index = "";
	return (setUp);
}

sockaddr* ConfigFile::convertToSockAddr(const std::string& ipAddress, int port)
{
	std::cout << "ipAddress---> " << ipAddress << std::endl;
	std::cout << "Port---> " << port << std::endl;
	if (port < 0 || port > 0XFFFF)
		throw std::runtime_error("ConfigFile:	convertToSockAddr:	Invalid port number");
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	int status = getaddrinfo(ipAddress.c_str(), NULL, &hints, &res);
	if (status != 0)
		throw std::runtime_error("ConfigFile:	convertToSockAddr:	Failed to get address info: ");
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next) 
	{
		if (p->ai_family == AF_INET) 
		{ // IPv4
			struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
			struct sockaddr_in* ipv4Copy = new sockaddr_in(*ipv4);
			freeaddrinfo(res);
			ipv4Copy->sin_port = htons(port); // Convert port to network byte order
			return (struct sockaddr*)ipv4Copy;
		} 
		else if (p->ai_family == AF_INET6) 
		{ // IPv6
			struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
			struct sockaddr_in6* ipv6Copy = new sockaddr_in6(*ipv6);
			freeaddrinfo(res);
			ipv6->sin6_port = htons(port); // Convert port to network byte order
			return (struct sockaddr*)ipv6Copy;
		}
	}
	freeaddrinfo(res);
	throw std::runtime_error("ConfigFile:	convertToSockAddr:	Failed to convert address to sockaddr");
}

bool dotCheck(int *dot, std::string address, int idx)
{
	if (address[idx] == '.')
		(*dot)++;
	if (address[idx] == '.' && address[idx + 1] && address[idx + 1] == '.')
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[idx] << " <--" <<std::endl;
		return (false);
	}
	else if (*dot > 3)
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[idx] << " <--" <<std::endl;
		return (false);
	}
	return (true);
}

bool	checkAddress(std::string address)
{
	int i;
	int dot;

	i = 0;
	dot = 0;
	if (address[i] == '.')
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[i] << " <--" <<std::endl;
		return (false);
	}
	while (address[i])
	{
		if (isdigit(address[i]) || address[i] == '.')
		{
			if (!dotCheck(&dot, address, i))
				return (false);
		}
		else
		{
			std::cout << "Invalid format " << address << " " << "--> " << address[i] << " <--" << std::endl;
			return  (false);
		}
		i++;
	}
	if (dot != 3)
	{
		std::cout << "Invalid format " << address << std::endl;
		return (false);
	}
	return (true);
}

std::string	insertAddress(configServer &server, std::string address)
{
	if (!checkAddress(address))
	{
		server.validFormat = false;
		return ("");
	}
	else
		return (address);
}

bool checkPortValue(int port)
{
	if (port >= 0 && port <= MAX_PORT)
		std::cout << "port in range" << std::endl;
	else
	{
		std::cout << "port not in range" << std::endl;
		return (false);
	}
	return (true);
}

int	addPort(configServer &server, std::string address)
{
	char* endptr;
	int port = std::strtol(address.c_str(), &endptr, 10);
	if (*endptr)
	{
		std::cout << "Please add a valid port" << std::endl;
		server.validFormat = false;
		return (-1);
	}
	if (checkPortValue(port))
		return (port);
	else
	{
		server.validFormat = false;
		return (-1);
	}	
}

void	ConfigFile::addAddress(configServer &server, std::istringstream &find)
{
	std::string tmp;
	std::string address;
	if (find >> tmp)
	{
		std::istringstream iss(tmp);
		getline(iss,address, ':');
		const std::string socketAddress = insertAddress(server, address);
		if (server.validFormat == false)
			return ;
		getline(iss,address,':');
		int port = addPort(server,address);
		server._socketAddress.interfaceAddress = convertToSockAddr(socketAddress, port);
		server._socketAddress.protocol = TCP;
		if (server.validFormat == false)
			return ;
	}
	else
	{
		std::cerr << "invalid format" << std::endl;
		server.validFormat = false;
	}
}
