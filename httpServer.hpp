#ifndef HTTPSERVER_CPP
#define HTTPSERVER_CPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
// #include <linux/io.h>
#include <arpa/inet.h>
#include <sstream>
#include <errno.h>
#define _XOPEN_SOURCE_EXTENDED 1
#define _OE_SOCKETS


//169.245.240.99
// struct sockaddr
// {
//     unsigned short integer sa_family;     /* address family */
//     char sa_data[14];      /*  up to 14 bytes of direct address */
// };
// struct in_addr
// {
//         unsigned long integer s_addr;
// };
// struct sockaddr_in
// {
//         short   sin_family;   //=IPv4 or IPv6 ...
//         unsigned short integer sin_port;
//         struct  in_addr sin_addr;
//         char    sin_zero[8];
// };

class	httpServer
{
	public:
		httpServer(std::string ip_address, int port);
		~httpServer();
		void	startListen();
		int		startServer();
		void	closeServer();
	
	private:
		int 				m_socket_fd;
		struct sockaddr_in 	m_socketAddress;
		std::string 		m_ip_address;
		int					m_port;
		unsigned int 		m_socketAddress_len;
		//int m_new_socket;
		// long 				m_incomingMessage;
		std::string 		m_serverMessage;

};
void exitWithError(const std::string &errorMessage);
void log(const std::string &message);

// char PURPLE=\033[0;95m
// GREEN=\033[92m
// CYAN=\033[96m
// BLUE=\033[94m
// WHITE=\033[0m
// RED=\033[91m
// YELLOW=\033[33m
// DF=\033[39m #DEFAULT

#endif