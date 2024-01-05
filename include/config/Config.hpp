#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../webserv.hpp"
#include "../error/Error.hpp"

class Config
{
	public:
        std::map<std::string, std::string>  _mimeTypes;

		Config();
        std::map<std::string, std::string>  parseMime();
        void                createKeypairs(std::map<std::string, std::string> & map, std::string s1, std::string s2);
};

#endif