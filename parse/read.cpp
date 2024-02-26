#include "configFile.hpp"

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

bool readConfig(int argc, char **argv, std::map<std::string, configServer> & config)
{
	if (argc == 2)
	{
		ConfigFile test;
		std::map<std::string, configServer> set;
		std::string file = stringConvert(argv[1]);
		std::cout << file << std::endl;
		set = test.readFile(file);
		test.setConFile(set);
		// std::map<std::string, configServer> out;
		config = test.getConFile();
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
