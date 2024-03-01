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
#define EnvCgi	map<string, string>

#define MAX_BODY_SIZE 50000

class Request
{
	// static	Config	_config;
	string			_request; // to add in cgi
	string			_method;	// to add in cgi
	string			_URI; // to add in cgi
	// string			_prefixPath;
	string			_httpVersion;
	string			_contentLength;
	string			_contentType;
	string			_date;
	string			_body;
	Headers			_headers;
	Query			_query;
	EnvCgi				_envCgi;
	void			parseMainHeader();
	void			parseHeaders();
	int				parseBody();
	bool			_sizeInRange;
	// string			parseHeader(string s);


	public:
		int				findDoubleNewline(std::string & s);
		Request(const string & request);
		friend ostream & operator<<(ostream & os, const Request & r);
		// void printHeaders();

		const string & 	getMethod()	{ return _method; }
		const string & 	getURI()	{ return _URI; }
		const bool   &	getSizeBound(){ return _sizeInRange; }
		Headers & 		getHeaders(){ return _headers; }
		const string & 	getBody()	{ return _body; }
		void			parseQuery(const string & path);
		const Query &	getQuery(){ return _query;}
		void 			buildCgiEnv(void);
		int				executeCgi(std::string &cgiScript);
};

#define BLUECOLOR "\033[1;96m"
#define MAGCOLOR "\033[1;95m"
#define NORM "\033[0m"

#endif