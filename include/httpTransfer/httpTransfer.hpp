#ifndef TRANSFER_HPP
#define TRANSFER_HPP

#include "../webserv.hpp"
#include "../error/Error.hpp"
#include "config/Config.hpp"

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
        // std::string              readRequest();
        void                parseHeader();

		static Config		_config;
    public:
        httpTransfer(const std::string & request);
    	std::string				exchange();
};

#endif