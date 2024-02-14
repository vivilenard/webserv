#include "Server.hpp"
#include "parse/configFile.hpp"
//int socket(int domain, int type, int protocol);
// 'IPv4' defined by 'AF_INET', 'SOCK_STREAM' for bidirectional TCP traffic, '0'=default protocol for type
// multiplexing

std::string stringConvert(char *input)
{
	std::stringstream format;
	format << input;
	std::string convert = format.str();
	if (!convert.empty())
		return (convert);
	else
		return ("");
}


int main(int argc, char **argv)
{
	if (argc == 2)
	{
		ConfigFile test;
		std::map<std::string, configServer> set;
		std::string file = stringConvert(argv[1]);
		std::cout << file << std::endl;
		set = test.readFile(file);
		test.setConFile(set);
		std::map<std::string, configServer> out;
		out = test.getConFile();
	}
	else if (argc == 1)
	{
		ConfigFile test;
		test.runByDefault();
		std::map<std::string, configServer> out;
		out = test.getConFile();
		std::cout << "server name " << out["default"]._serverName << std::endl;
		std::cout << "location name " << out["default"]._locations["default"]._name << std::endl;
	}
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