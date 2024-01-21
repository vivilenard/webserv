#include "Server.hpp"

Server::Server(std::string ip_address, int port):
	_socket_fd(), _new_socket_fd() ,_socketAddress(), _ip_address(ip_address), 
	_port(port), _socketAddress_len(sizeof(_socketAddress))
{
	_socketAddress.sin_family = AF_INET;  //address family that is used to designate the type of addresses that your socket can communicate with
	_socketAddress.sin_port = htons(_port); //htons: from host to network byte order
	_socketAddress.sin_addr.s_addr = inet_addr(_ip_address.c_str());
	cout << "Server created" << endl;
}

int Server::startServer()
{

	//create socket
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (_socket_fd < 0)
		return exitWithError("Cannot create socket"), 1;

	//bind it to address
	if (bind(_socket_fd,(sockaddr *)&_socketAddress, _socketAddress_len) < 0)
		exitWithError("Cannot connect socket to address");
	cout << "Server started" << endl;
	return 0;
}

void Server::startListen()
{
	//listens for incoming connections
	if (listen(_socket_fd, 240) < 0)
		exitWithError("Socket listen failed");
	std::ostringstream ss;
	ss << "\nListening to Address: "
		<< inet_ntoa(_socketAddress.sin_addr) //networkbyteorder to ipv4 address
		<< "\n on Port "
		<< ntohs(_socketAddress.sin_port) //converts the unsigned short integer netshort from network byte order to host byte order.
		<< "\n\n";
	cout << ss.str() << endl;
}

void Server::acceptConnection()
{
	while (1)
	{
		//creates new socket with connection thread. Saves the new sockets address in _socketAddress as we no longer need the new one
		_new_socket_fd = accept(_socket_fd, (sockaddr *)&_socketAddress, 
							&_socketAddress_len);
		if (_new_socket_fd < 0)
			connectionError();
		httpTransfer transfer;
		transfer.exchange(_new_socket_fd);
	}
}

void	Server::connectionError()
{
	std::ostringstream ss;
	ss << "Server failed to accept incoming connection from ADDRESS: " 
	<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
	<< ntohs(_socketAddress.sin_port);
	exitWithError(ss.str());
}

Server::~Server()
{
	cout << "TcpServer destructed" << endl;
}

void Server::closeServer()
{
	for (int i = getdtablesize(); i > 3;) close(--i);
		exit(0);
}
