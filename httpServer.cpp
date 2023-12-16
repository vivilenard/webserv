#include "httpServer.hpp"
using namespace std;

httpServer::httpServer(std::string ip_address, int port):
	m_ip_address(ip_address), m_port(port), m_socketAddress_len(sizeof(m_socketAddress))
{

	m_socketAddress.sin_family = AF_INET;
	m_socketAddress.sin_port = htons(m_port);
	m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
	cout << "TcpServer created" << endl;
}

int httpServer::startServer()
{
	int m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket < 0)
	{
		exitWithError("Cannot create socket");
		return 1;
	}
	if (bind(m_socket,(sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
	{
		exitWithError("Cannot connect socket to address");
		return 1;
	}
		return 0;
}

httpServer::~httpServer()
{
	cout << "TcpServer destructed" << endl;
}

void httpServer::closeServer()
	{
		close(m_socket);
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