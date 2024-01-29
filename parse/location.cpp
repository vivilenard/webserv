#include "configFile.hpp"

void	ConfigFile::setMethod(configServer &server, std::string dir,
							  					std::string rule)
{
	std::string divider;
	std::string method;
	std::istringstream ruleStream(rule);
	ruleStream >> divider;
	ruleStream >> method;
	std::cout << "RULE " << method << std::endl;
	if (method == "PUT")
		server._locations[dir]._put = true;
	else if (method == "GET")
		server._locations[dir]._get = true;
	else if (method == "POST")
		server._locations[dir]._post = true;

}

std::string	ConfigFile::addLocation(configServer &server, std::string token, std::istringstream &find)
{
	if (token == "location")
	{
		std::string dir;
		if (find >> dir)
		{
			server._locations[dir]._name = dir;
			return (server._locations[dir]._name);
		}
		else
		{
			server.validFormat = false;
			return("");
		}
	}
	return ("");
}