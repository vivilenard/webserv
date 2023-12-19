#include "httpServer.hpp"
using namespace std;

httpServer::httpServer(std::string ip_address, int port):
	_socket_fd(), _socketAddress(), _ip_address(ip_address), 
	_port(port), _socketAddress_len(sizeof(_socketAddress))
{
	_socketAddress.sin_family = AF_INET;
	_socketAddress.sin_port = htons(_port);
	_socketAddress.sin_addr.s_addr = inet_addr(_ip_address.c_str());
	cout << "TcpServer created" << endl;
}

int httpServer::startServer()
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd < 0)
	{
		exitWithError("Cannot create socket");
		return 1;
	}
	if (bind(_socket_fd,(sockaddr *)&_socketAddress, _socketAddress_len) < 0)
	{
		exitWithError("Cannot connect socket to address");
		return 1;
	}
	log("Server started");
	return 0;
}

void httpServer::startListen()
{
	cout << "File descriptor" << _socket_fd << endl;
	if (listen(_socket_fd, 2) < 0)
	{
		perror("listen");
		exitWithError("Socket listen failed");
	}
	std::ostringstream ss;
	ss << "\nListening to Address: "
		<< inet_ntoa(_socketAddress.sin_addr) //networkbyteorder to ipv4 address
		<< "\n on Port "
		<< ntohs(_socketAddress.sin_port) //converts the unsigned short integer netshort from network byte order to host byte order.
		<< "\n\n";
	log(ss.str());
}

httpServer::~httpServer()
{
	cout << "TcpServer destructed" << endl;
}

void httpServer::closeServer()
	{
		close(_socket_fd);
		//close(_new_socket);
		exit(0);
	}

void httpServer::acceptConnection()
{
	
}

void log(const std::string &message)
{
	std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage)
{
	log("ERROR: " + errorMessage);
	exit(1);
}