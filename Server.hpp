#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "webserv.hpp"
#include "Error.hpp"
#include "httpTransfer.hpp"

class httpTransfer;

class	Server
{
	public:
		Server(std::string ip_address, int port);
		~Server();
		void	startListen();
		int		startServer();
		void	acceptConnection();
		void	closeServer();

		//Errors
		void	connectionError();
	
	private:
		int 				_socket_fd;
		int 				_new_socket_fd;
		struct sockaddr_in 	_socketAddress;
		std::string 		_ip_address;
		int					_port;
		unsigned int 		_socketAddress_len;
		std::string 		_serverMessage;

};
void exitWithError(const std::string &errorMessage);




// char PURPLE=\033[0;95m
// GREEN=\033[92m
// CYAN=\033[96m
// BLUE=\033[94m
// WHITE=\033[0m
// RED=\033[91m
// YELLOW=\033[33m
// DF=\033[39m #DEFAULT

#endif