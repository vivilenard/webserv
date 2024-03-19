#include "configFile.hpp"

void	ConfigFile::setMethod(configServer &server, std::string dir,
							  					std::string rule)
{
	std::string divider;
	std::string methodLine;
	std::string methodStream;
	std::string	method;
	std::istringstream ruleStream(rule);
	ruleStream >> divider;
	if (divider == "allow_methods")
	{
		std::getline(ruleStream, methodLine);
		std::istringstream methodStream(methodLine);
		while (methodStream >> method)
		{
			if (method == "DELETE")
				server._locations[dir]._delete = true;
			else if (method == "GET")
				server._locations[dir]._get = true;
			else if (method == "POST")
				server._locations[dir]._post = true;
			else
			{
				std::cerr << "Wrong method ---> " << method << " <---"<< std::endl;
				server._locations[dir]._delete = false;
				server._locations[dir]._get = false;
				server._locations[dir]._post = false;
			}
		}
	}
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

void ConfigFile::hasRedirection(configServer &server, std::string dir, std::string rule)
{
	std::string divider;
	std::istringstream ruleStream(rule);
	std::string redirectionPath;
	std::string redirLine;
	if (ruleStream >> divider)
	{
		if (divider == "redirection")
		{
			std::getline(ruleStream, redirLine);
			std::istringstream redirStream(redirLine);
			if (redirStream >> redirectionPath)
			{
				server._locations[dir]._redirect = redirectionPath;
			}
			else
				server._locations[dir]._redirect = "";

		}
	}
}

bool	ConfigFile::setLocation(configServer &server, std::ifstream &inputFile,
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
			hasRedirection(server, dir,line2);
			if (token1 == "}")
			{
				line += line2;
				break ;
			}
		}
		return (true);
	}
	return (false);
}
