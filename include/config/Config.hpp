#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../webserv.hpp"
#include "../error/Error.hpp"

class Config
{
	private:
		const string _rootPath;
		string _defaultPath;
	public:
        std::map<std::string, std::string>  _mimeTypes;
		string	prefixPath;
		string	prefixPostPath;

		Config();
        std::map<std::string, std::string>  parseMime();
        void createKeypairs(std::map<std::string, std::string> & map, std::string s1, std::string s2);

		const string getRootPath() const { return _rootPath; };
		const string getDefaultPath() const { return _defaultPath; };
};

#endif