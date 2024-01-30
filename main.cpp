#include "Server.hpp"
#include "parse/configFile.hpp"
//int socket(int domain, int type, int protocol);
// 'IPv4' defined by 'AF_INET', 'SOCK_STREAM' for bidirectional TCP traffic, '0'=default protocol for type
// multiplexing



int main()
{
	ConfigFile test;
	std::map<std::string, configServer> set;
	set = test.readFile("example.conf");
	test.setConFile(set);
	std::map<std::string, configServer> out;
	out = test.getConFile();
	std::cout << "THIS FROM OUT " << out["example.com"]._serverName << std::endl;
	std::cout << "THIS FROM OUT " << out["example.com"]._root << std::endl;
	std::cout << "THIS FROM OUT " << out["example.com"]._locations["/put_test"]._name << std::endl;
	std::cout << "LOCATION PUT_TEST PUT " << out["example.com"]._locations["/put_test"]._put << std::endl;
	std::cout << "THIS FROM PUT_TEST POST " << out["example.com"]._locations["/put_test"]._post << std::endl;
	std::cout << "LOCATION PUT_TEST GET " << out["example.com"]._locations["/put_test"]._get << std::endl;
	/*std::cout << "THIS FROM PUT_TEST POST " << out["example.com"]._locations["/put_test"]._post << std::endl;*/
	std::cout << "THIS FROM POST_BODY POST " << out["example.com"]._locations["/post_body"]._post << std::endl;
	std::cout << "THIS FROM POST_BODY POST " << out["example.com"]._locations["/post_body"]._get << std::endl;
	std::cout << "THIS FROM POST_BODY POST " << out["example.com"]._locations["/post_body"]._put << std::endl;

/*	Server serv = Server("0.0.0.0", 8080);
	serv.startServer();
	serv.startListen();
	serv.acceptConnection();
	serv.closeServer();*/
	return 0;
}

// run program
// open other terminal
// $: telnet localhost 8080
// type some message