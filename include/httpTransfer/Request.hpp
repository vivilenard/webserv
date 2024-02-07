#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"
#include "../error/Error.hpp"
#include "config/Config.hpp"

#include <fstream>
#include <fcntl.h>
#include <algorithm>

#define Headers map<string, string>
#define Query map<string, string>
#define MAX_BODY_SIZE 50000

class Request
{
	// static	Config	_config;
	string			_request;
	string			_method;
	string			_path;
	// string			_prefixPath;
	string			_httpVersion;
	string			_contentLength;
	string			_contentType;
	string			_date;
	string			_body;
	Headers			_headers;
	Query			_query;
	void			parseMainHeader();
	void			parseHeaders();
	int				parseBody();
	int				findDoubleNewline(std::string & s);
	bool			_sizeInRange;
	// string			parseHeader(string s);


	public:
		Request(const string & request);
		friend ostream & operator<<(ostream & os, const Request & r);
		// void printHeaders();

		const string & 	getMethod()	{ return _method; }
		const string & 	getPath()	{ return _path; }
		const bool   &	getSizeBound(){ return _sizeInRange; }
		Headers & 		getHeaders(){ return _headers; }
		const string & 	getBody()	{ return _body; }
		void			parseQuery(const string & path);


};

#define BLUECOLOR "\033[1;96m"
#define MAGCOLOR "\033[1;95m"
#define NORM "\033[0m"

#endif