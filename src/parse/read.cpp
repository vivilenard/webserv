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
		std::string file = "config/default.conf";
		test.readFile(file, config);
		if (config.empty())
			return (cout << "No default config file found. Please create one in: config/default.conf" << endl, false);
		return true;
	}
	return false;
}
