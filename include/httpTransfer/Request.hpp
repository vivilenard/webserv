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
#define PAIR pair<string, string>

// static string Request::_boundary = "";

class Request
{
	// static	Config	_config;
	bool			_standardRequest;
	string			_request;
	string			_method;
	string			_URI;
	// string			_prefixPath;
	string			_httpVersion;
	string			_contentLength;
	string			_contentType;
	string			_date;
	string			_body;
	Headers			_headers;
	Query			_query;
	bool			_sizeInRange;
	void			parseMainHeader();
	void			parseHeaders(Headers & headers);
	int				parseBody(string & body);
	int				findDoubleNewline(std::string & s);
	bool			isMultipartChunk();
	bool			ContainsMultipartHeader();
	bool			handleMultipart();
	bool			setBoundary();
	PAIR 			parsePair(string line);
	void			identifyRequest();
	// string			parseHeader(string s);


	public:
		static stringstream 	MultipartBody;
		static string			MultipartContentType;
		static string			boundary; //if boundary is cleared, body is ready to be used.
											//and treated as normal post.
		Request(const string & request);
		friend ostream & operator<<(ostream & os, const Request & r);
		// void printHeaders();

		const string & 	getMethod()	{ return _method; }
		const string & 	getURI()	{ return _URI; }
		const bool   &	getSizeBound(){ return _sizeInRange; }
		Headers & 		getHeaders(){ return _headers; }
		const string & 	getBody()	{ return _body; }
		void			parseQuery(const string & path);


};

#define BLUECOLOR "\033[1;96m"
#define COL "\033[1;91m"
#define RED "\033[1;93m"
#define GREEN "\033[1;92m"
#define MAGCOLOR "\033[1;95m"
#define NORM "\033[0m"

#endif