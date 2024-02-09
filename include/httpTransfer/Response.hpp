#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "config/Config.hpp"


class Response
{
	string			_response;
	static Config 	_config;
	Request			_request;
	string			_status;
	string			_statusInfo;
	string			_httpVersion;
	string			_newPath;
	string			_contentType;
	string			_contentLenght;
	string			_responseBody;

	void		processGet();
	void		processPost();
	void		processDelete();
	string		readFile(const string & path);
	string		addRootPath(const string & path);
	string		getContentType(const string & path);
	int			createFile(std::string & path, std::string & cont);
	std::string	findKeyByValue(std::map<string, string>m, string value);
	void		formResponse(const string & status);
	bool		isMultipart();
	bool		isCgi(const string & path);

	public:
		Response(Request & request);
	// static Config 	_config;

		const string getResponse() const { return _response ; };
};


#endif