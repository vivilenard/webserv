#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "config/Config.hpp"
#include "Status.hpp"
#include <map>



class Response
{
	Request			_request;
	string			_response;
	static Config 	_config;
	static StatusCode _statusCode;
	int				_status;
	string			_statusInfo;
	string			_httpVersion;
	string			_URI;
	string			_responseBody;
	string			_fileContent;
	string			_fileContentType;
	string			_cgiScript;

	void			processGet();
	void			processPost();
	void			processDelete();
	bool			readFile(const string & path);
	string			addRootPath(const string & path);
	int				addDefaultFile(string & path);
	string			getContentType(const string & path);
	int				createFile(std::string & path, const std::string & cont);
	std::string		findKeyByValue(std::map<string, string>m, string value);
	void			formResponse(const int & status, const string & statusInfo);
	const string	createErrorBody(const int & status, const string & statusInfo);
	bool			isMultipart();
	bool			isCgi(const string & path);

	public:
		Response(Request & request);
		const string getResponse() const { return _response ; };
};

int executeCgi(std::string &cgiScript);
#endif