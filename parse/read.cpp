#include "configFile.hpp"

std::string stringConvert(char *input)
{
	if (input)
		return (std::string (input));
	else
		return ("");
}

bool readConfig(int argc, char **argv, std::map<std::string, configServer> & config)
{
	// cout << "read config" << endl;
	if (argc == 2)
	{
		ConfigFile test;
		std::string file = std::string(argv[1]);
		if (file.empty())
			return (false);
		std::cout << file << std::endl;
		test.readFile(file, config);
		if (config.empty())
			return false;
		// std::map<std::string, configServer> out;
		return true;
	}
	else if (argc == 1)
	{
		ConfigFile test;
		test.runByDefault();
		// std::map<std::string, configServer> out;
		config = test.getConFile();
		std::cout << "server name " << config["default"]._serverName << std::endl;
		std::cout << "location name " << config["default"]._locations["default"]._name << std::endl;
		return true;
	}
	return false;
}
