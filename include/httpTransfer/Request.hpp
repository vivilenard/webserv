#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../webserv.hpp"

#include <fstream>
#include <fcntl.h>
#include <algorithm>

#define Headers map<string, string>
#define Query map<string, string>
#define MAX_BODY_SIZE 1000000
#define PAIR pair<string, string>

class Request
{
	bool			_standardRequest;
	string			_request;
	string			_method;
	string			_URI;
	string			_httpVersion;
	string			_contentLength;
	string			_contentType;
	string			_date;
	string			_body;
	string			_filename;
	Headers			_headers;
	Query			_query;
	bool			_sizeInRange;

	void			parseMainHeader();
	void			parseHeaders(Headers & headers);
	int				parseBody(string & body, const string & chunk, const int & length);
	int				findDoubleNewline(const std::string & s);
	PAIR 			parsePair(string line);
	void			identifyRequest();
	bool			isMultipartChunk();
	bool			ContainsMultipartHeader();
	bool			handleMultipart();
	bool			parseMultipart(const string & s);
	bool			setBoundary();
	bool			ApproveMultipart(const string preBoundary, const string bound);
	const string	setBoundaryTogether(const string & bound, const string & type);
	void			clearMultipartData();
	void			setFilename();
	void			packTogether();
	bool			checkRequestSize();
	void			parseContentAttributes(Headers & attributes, const string & s);

	public:
		static int				MultipartApproved;
		static string			MultipartName;
		static string			MultipartBody;
		static string			MultipartContentType;
		static int				McontentLength;
		static string			boundary; //if boundary is cleared, body is ready to be used.
											//and treated as normal post.
		Request(const string & request);
		friend ostream & operator<<(ostream & os, const Request & r);

		const string & 	getMethod()	{ return _method; }
		const string & 	getURI()	{ return _URI; }
		const string & 	getFilename()	{ return _filename; }
		const bool   &	getSizeInRange(){ return _sizeInRange; }
		Headers & 		getHeaders(){ return _headers; }
		const string & 	getBody()	{ return _body; }
		void			parseQuery(const string & path);


};

#define RED "\033[1;91m"
#define GREEN "\033[1;92m"
#define ORANGE "\033[1;93m"
#define MAG "\033[1;95m"
#define BLUE "\033[1;96m"
#define NORM "\033[0m"
#define BACK "\033[45m"

#endif