#include "httpServer.hpp"

//int socket(int domain, int type, int protocol);
// 'IPv4' defined by 'AF_INET', 'SOCK_STREAM' for bidirectional TCP traffic, '0'=default protocol for type

int main()
{
	httpServer serv = httpServer("0.0.0.0", 8080);
	serv.startServer();
	serv.startListen();
	return 0;
}