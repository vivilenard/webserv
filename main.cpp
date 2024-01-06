#include "Server.hpp"

//int socket(int domain, int type, int protocol);
// 'IPv4' defined by 'AF_INET', 'SOCK_STREAM' for bidirectional TCP traffic, '0'=default protocol for type
// multiplexing



int main()
{
	Server serv = Server("0.0.0.0", 8080);
	serv.startServer();
	serv.startListen();
	serv.acceptConnection();
	serv.closeServer();
	return 0;
}

// run program
// open other terminal
// $: telnet localhost 8080
// type some message