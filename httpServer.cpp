#include "httpServer.hpp"
using namespace std;

httpServer::httpServer(std::string ip_address, int port):
	_socket_fd(), _new_socket_fd() ,_socketAddress(), _ip_address(ip_address), 
	_port(port), _socketAddress_len(sizeof(_socketAddress))
{
	_socketAddress.sin_family = AF_INET;  //address family that is used to designate the type of addresses that your socket can communicate with
	_socketAddress.sin_port = htons(_port); //htons: from host to network byte order
	_socketAddress.sin_addr.s_addr = inet_addr(_ip_address.c_str());
	cout << "TcpServer created" << endl;
}

int httpServer::startServer()
{

	//create socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (_socket_fd < 0)
		return exitWithError("Cannot create socket"), 1;

	//bind it to address
	if (bind(_socket_fd,(sockaddr *)&_socketAddress, _socketAddress_len) < 0)
		exitWithError("Cannot connect socket to address");
	log("Server started");
	return 0;
}

void httpServer::startListen()
{
	
	cout << "File descriptor" << _socket_fd << endl;

	//listens for incoming connections
	if (listen(_socket_fd, 240) < 0)
		exitWithError("Socket listen failed");
	std::ostringstream ss;
	ss << "\nListening to Address: "
		<< inet_ntoa(_socketAddress.sin_addr) //networkbyteorder to ipv4 address
		<< "\n on Port "
		<< ntohs(_socketAddress.sin_port) //converts the unsigned short integer netshort from network byte order to host byte order.
		<< "\n\n";
	log(ss.str());
}

void httpServer::acceptConnection()
{
	//creates new socket with connection thread. Saves the new sockets address in _socketAddress as we no longer need the new one
	_new_socket_fd = accept(_socket_fd, (sockaddr *)&_socketAddress, 
                        &_socketAddress_len);
	
	if (_new_socket_fd < 0)
    {
        std::ostringstream ss;
        ss << 
        "Server failed to accept incoming connection from ADDRESS: " 
        << inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
        << ntohs(_socketAddress.sin_port);
        exitWithError(ss.str());
    }

	//reads
	const int BUFFER_SIZE = 30720;

	char buffer[BUFFER_SIZE] = {0};
	int bytesReceived = read(_new_socket_fd, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		exitWithError("Failed to read bytes from client socket connection");
	
	cout << "RECIEVED: \n" << buffer << endl;

	std::string content_length;
	std::string status = "HTTP/1.1 200 OK\r\n";
	std::string type = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	std::string content = "<h1> hii </h1>";
	std::string connection = "Connection: close\r\n";
	std::string length = "Content-Length: 14\r\n";

	std::string response = status + length + connection + type + content;
	std::string r1 = "HTTP/1.1 200 OK\r\nServer: Apache/1.3.3.7 (Unix) (Red-Hat/Linux)\r\nContent-Length: 11\r\nConnection: Closed\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<h1>hi</h1>";
	std::cout << "RESPONSE: \n" << r1 << endl; 
	write(_new_socket_fd, response.c_str(), sizeof(response));
}

void log(const std::string &message)
{
	std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage)
{
	log("ERROR: " + errorMessage);
	for (int i = getdtablesize(); i > 3;) close(--i);
	exit(1);
}

httpServer::~httpServer()
{
	cout << "TcpServer destructed" << endl;
}

void httpServer::closeServer()
{
	for (int i = getdtablesize(); i > 3;) close(--i);
		exit(0);
}