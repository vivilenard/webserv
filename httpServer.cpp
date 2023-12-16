#include "httpServer.hpp"
using namespace std;

httpServer::httpServer(std::string ip_address, int port):
	m_ip_address(ip_address), m_port(port), m_socketAddress_len(sizeof(m_socketAddress))
{

	m_socketAddress.sin_family = AF_INET;
	m_socketAddress.sin_port = htons(m_port);
	m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
    memset(m_socketAddress.sin_zero, '\0', sizeof m_socketAddress.sin_zero);
	cout << "TcpServer created" << endl;
}

int httpServer::startServer()
{
	int m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket_fd < 0)
	{
		exitWithError("Cannot create socket");
		return 1;
	}
	if (bind(m_socket_fd,(sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
	{
		exitWithError("Cannot connect socket to address");
		return 1;
	}
	log("Server started");
	return 0;
}

void httpServer::startListen()
{

	if (listen(m_socket_fd, 2) < 0)
	{
		perror("listen");
		exitWithError("Socket listen failed");
	}
	std::ostringstream ss;
	ss << "\nListening to Address: "
		<< inet_ntoa(m_socketAddress.sin_addr) //networkbyteorder to ipv4 address
		<< "\n on Port "
		<< ntohs(m_socketAddress.sin_port) //converts the unsigned short integer netshort from network byte order to host byte order.
		<< "\n\n";
	log(ss.str());
}

httpServer::~httpServer()
{
	cout << "TcpServer destructed" << endl;
}

void httpServer::closeServer()
	{
		close(m_socket_fd);
		//close(m_new_socket);
		exit(0);
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