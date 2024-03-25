#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "webserv.hpp"
#include "configFile.hpp"


#include <fstream>
#include <fcntl.h>
#include <algorithm>
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
#include <fstream>
#include <sstream>
#include <ctime>

#define stringMAP map<string, string>
#define COOKIES map<string, string>
#define Headers map<string, string>
#define Query map<string, string>
#define EnvCgi	map<string, string>
#define PAIR pair<string, string>


#define MAX_BODY_SIZE 50000
#define PRINT false

class Request
{
	configServer	_configfile;
	bool			_standardRequest;
	string			_request;
	string			_method;
	string			_URI;
	string			_httpVersion;
	stringMAP		_cookies;
	string			_contentLength;
	string			_contentType;
	string			_date;
	string			_body;
	string			_filename;
	Headers			_headers;
	Query			_query;
	EnvCgi			_envCgi;
	bool			_sizeInRange;

	void			parseMainHeader();
	void			parseHeaders(Headers & headers, const string & chunk);
	int				parseBody(string & body, const string & chunk, const int & length);
	PAIR 			parsePair(string line);
	void			parseCookies(string cookieDough);
	void			identifyRequest();
	bool			ContainsMultipartHeader();
	bool			handleMultipart();
	bool			parseMultipart(const string & s);
	bool			setBoundary();
	bool			ApproveMultipart(const string preBoundary, const string bound);
	const string	setBoundaryTogether(const string & bound, const string & type);
	void			clearMultipartData();
	void			setFilename();
	void			setName(const string & attr, Headers & attributes);
	void			packTogether();
	bool			checkRequestSize();
	void			parseContentAttributes(stringMAP & attributes, const string & s);

	public:

		int						findDoubleNewline(const std::string & s);
		static int				MultipartApproved;
		static string			MultipartName;
		static string			MultipartURI;
		static string			MultipartBody;
		static string			MultipartContentType;
		static int				McontentLength;
		static string			boundary; //if boundary is cleared, body is ready to be used.
											//and treated as normal post.
		Request(const string & request, configServer & configfile);
		friend ostream & operator<<(ostream & os, const Request & r);

		const string &		getRequest()	{ return _request; }
		const string &		getMethod()		{ return _method; }
		const string &		getURI()		{ return _URI; }
		const string &		getFilename()	{ return _filename; }
		const bool   &		getSizeInRange(){ return _sizeInRange; }
		Headers & 			getHeaders()	{ return _headers; }
		const string &		getBody()		{ return _body; }
		const stringMAP&	getCookies()	{ return _cookies; }
		Query &				getQuery()		{ return _query;}
		void				parseQuery(const string & path);
		void 				buildCgiEnv(void);
		int					executeCgi(std::string &cgiScript, const string & path);
};


void printMAP(stringMAP m);

#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define ORANGE "\033[1;93m"
#define MAG "\033[1;95m"
#define BLUE "\033[1;96m"
#define NORM "\033[0m"
#define BACK "\033[45m"

#endif
