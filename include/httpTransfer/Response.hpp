#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "config/Config.hpp"
#include "Status.hpp"
#include "../../parse/configFile.hpp"

#include <map>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

struct configServer;

#define STATUSCODE _statusCode.getStati()

class Response
{
	configServer	_configfile;
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

	bool			invalidRequest();
	void			processGet();
	void			processPost();
	void			processDelete();
	bool 			methodAllowed();
	bool			readFile(const string & path);
	string			addRootPath(const string & path);
	int				addDefaultFile(string & path);
	string			getContentType(const string & path);
	int				createFile(std::string & path, const std::string & cont);
	std::string		findKeyByValue(std::map<string, string>m, string value);
	void			formResponse(const int & status, const string & statusInfo);
	const string	createErrorBody(const int & status, const string & statusInfo);
	int				isMultipart();
	bool			isCgi(const string & path);
	int				handleMultipart();
	bool			incorrectMimeType(const string & contentType);
	bool			noFilename();
	bool			listDirectory(const string & endpoint);
	string			currentDir();


	public:
		Response(Request & request, configServer & configfile);
		const	string getResponse() const { return _response ; };
		bool	beginsWithDot(const string & s);
		string	getFileCreationTime(char *path);
};

#endif