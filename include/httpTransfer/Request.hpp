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
	void			parseBody();
	int				findDoubleNewline(std::string & s);
	// string			parseHeader(string s);


	public:
		Request(const string & request);
		friend ostream & operator<<(ostream & os, const Request & r);
		// void printHeaders();

		const string & 	getMethod()	{ return _method; }
		const string & 	getPath()	{ return _path; }
		Headers & 		getHeaders(){ return _headers; }
		const string & 	getBody()	{ return _body; }
		void			parseQuery(const string & path);


};


#endif