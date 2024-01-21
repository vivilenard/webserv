#ifndef TRANSFER_HPP
#define TRANSFER_HPP

#include "../webserv.hpp"
#include "../error/Error.hpp"
#include "config/Config.hpp"

#include <fstream>
#include <fcntl.h>

class httpTransfer
{
    private:
/*         int                 _socket_fd; */
        std::string              _mimeType;
        std::string              _request;
        std::string              _method;
		std::string  			 _prePath;
        std::string              _path;
        std::string              _httpVersion;
		int						_contentLength;
        // std::string              readRequest();
        void                    parseHeader();
        std::string             parseHeaderValue(std::string s);
        std::string             parseBody();
        int	                    findDoubleNewline(std::string & s);
        std::string             findKeyByValue(std::map<string, string>map, string value);
        int                     createFile(std::string & path, std::string & content, std::string & fileType);

		static Config		_config;
    public:
        httpTransfer(const std::string & request);
    	std::string	createResponse();
        std::string getRequest();
        std::string postRequest();
        std::string delRequest();
};

#endif