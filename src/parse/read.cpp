#include "configFile.hpp"

std::string stringConvert(char *input)
{
	if (input)
		return (std::string (input));
	else
		return ("");
}

bool serverValid(configServer server)
{
	if (server._root.empty())
	{
		server.validFormat = false;
		return (false);
	}
	return true;
}

bool checkValid(std::map<std::string, configServer> & config)
{
	for (CONFIG::iterator it2 = config.begin(); it2 != config.end(); it2++) {
		if (!serverValid(it2->second))
			return (false);	
	}
	return true;
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
		if (config.empty() || !checkValid(config))
		{
			cout << "Invalid config file." << endl;
			return false;
		}
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
		if (!checkValid(config))
		{
			cout << "Invalid config file." << endl;
			return false;
		}
		return true;
	}
	return false;
}
