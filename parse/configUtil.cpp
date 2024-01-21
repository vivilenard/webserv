#include "configFile.hpp"

void	ConfigFile::addAddress(configServer &server, std::istringstream &find)
{
	std::string tmp;
	std::string address;

	if (find >> tmp)
	{
		std::istringstream iss(tmp);
		getline(iss,address, ':');
		std::cout << address << std::endl;
		server._address = address;
		getline(iss,address, ':');
		server._listen = std::stoi(address);
		std::cout << server._listen << std::endl;
	}
	else
}