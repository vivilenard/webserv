#include "configFile.hpp"

bool dotCheck(int *dot, std::string address, int idx)
{
	if (address[idx] == '.')
		(*dot)++;
	if (address[idx] == '.' && address[idx + 1] && address[idx + 1] == '.')
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[idx] << " <--" <<std::endl;
		return (false);
	}
	else if (*dot > 3)
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[idx] << " <--" <<std::endl;
		return (false);
	}
	return (true);
}

bool	checkAddress(std::string address)
{
	int i;
	int dot;

	i = 0;
	dot = 0;
	if (address[i] == '.')
	{
		std::cout <<"Invalid format "<< address << " " << "--> " << address[i] << " <--" <<std::endl;
		return (false);
	}
	while (address[i])
	{
		if (isdigit(address[i]) || address[i] == '.')
		{
			if (!dotCheck(&dot, address, i))
				return (false);
		}
		else
		{
			std::cout << "Invalid format " << address << " " << "--> " << address[i] << " <--" << std::endl;
			return  (false);
		}
		i++;
	}
	if (dot != 3)
	{
		std::cout << "Invalid format " << address << std::endl;
		return (false);
	}
	return (true);
}

bool	insertAddress(configServer &server, std::string address)
{
	if (!checkAddress(address))
		return (false);
	else
		server._address = address;
	return (true);
}

bool addPorti(configServer &server, std::string address)
{
	char* endptr;
	int port = std::strtol(address.c_str(), &endptr, 10);
	server._listen = port;
	return (true);
}

void	ConfigFile::addAddress(configServer &server, std::istringstream &find)
{
	std::string tmp;
	std::string address;
	if (find >> tmp)
	{
		std::istringstream iss(tmp);
		getline(iss,address, ':');
		if (!insertAddress(server, address))
			return ;
		getline(iss,address, ':');
		addPorti(server, address);

	}
	else
		std::cerr << "invalid format" << std::endl;
}