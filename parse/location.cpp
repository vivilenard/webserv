#include "configFile.hpp"

void	ConfigFile::setMethod(configServer &server, std::string dir,
							  					std::string rule)
{
	std::string divider;
	std::string method;
	std::istringstream ruleStream(rule);
	ruleStream >> divider;
	ruleStream >> method;
	if (method == "DELETE")
		server._locations[dir]._delete = true;
	else if (method == "GET")
		server._locations[dir]._get = true;
	else if (method == "POST")
		server._locations[dir]._post = true;

}

std::string	ConfigFile::addNameLocation(configServer &server, std::string token, std::istringstream &find)
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

void	ConfigFile::setLocation(configServer &server, std::ifstream &inputFile,
										std::string token, std::string &line, std::istringstream &find)
{
	if (token == "location")
	{
		std::string dir = addNameLocation(server, token, find);
		std::string line2 = line;
		while (std::getline(inputFile, line2))
		{
			std::string token1;
			std::istringstream find1(line2);
			find1 >> token1;
			setMethod(server, dir, line2);
			if (token1 == "}")
			{
				line += line2;
				break ;
			}
		}
	}
}
