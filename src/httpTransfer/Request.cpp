#include "Request.hpp"
#include "configFile.hpp"

int		Request::MultipartApproved = 0;
string	Request::boundary = "bound";
string	Request::MultipartBody = "";
int		Request::McontentLength = 0;
string	Request::MultipartContentType = "";
string	Request::MultipartName = ".empty.";
string	Request::MultipartURI = "";

Request::Request(const string & request, configServer & configfile):  _configfile(configfile), _request(request), _filename(""), _sizeInRange(true)
{
	cout << "constructttttt" << endl;
	identifyRequest();
	if (_standardRequest)
	{
		parseMainHeader();
		parseHeaders(_headers, _request);
		parseBody(_body, _request, atoi(_headers["Content-Length"].c_str()));
	}
	checkRequestSize();
	handleMultipart();

	Query::iterator it = _query.begin();
	for (; it != _query.end(); it++)
	{
		cout << it->first << " " << it->second << endl;
	}
}

void	Request::parseMainHeader()
{
	istringstream istream(_request);
	getline(istream, _method, ' ');
	getline(istream, _URI, ' ');
	getline(istream, _httpVersion, ' ');
	_httpVersion = _httpVersion.substr(0, _httpVersion.find_first_of("\n"));
	size_t pos = _URI.find_first_of('?');
	if (pos != string::npos)
	{
		parseQuery(_URI.substr(pos + 1));
		_URI.erase(pos);
	}
}


void Request::identifyRequest()
{
	istringstream istream(_request);
	string buffer;
	getline(istream, buffer);
	if (buffer.find("HTTP/") != string::npos)
	{
		_standardRequest = true;
		clearMultipartData();
		return ;
	}
	_standardRequest = false;
}

void	Request::parseCookies(string cookieDough)
{
	if (cookieDough.empty())
		return ;
	// cout << "PARSE COOKIES" << endl;
	// cout << GREEN << cookieDough << NORM << endl;
	string buffer;
	istringstream is(cookieDough);
	while (getline(is, buffer, ';'))
		parseContentAttributes(_cookies, buffer);
	// cout << "parsed COOKIES:" << endl;
	// stringMAP::iterator it = _cookies.begin();
	// for (; it != _cookies.end() ; it++)
	// {
	// 	cout << GREEN << it->first << " = " << it->second << NORM << endl;
	// }
	// cout << "printed" << endl;
}

void	Request::parseHeaders(Headers & headers, const string & chunk)
{
	PAIR p;
	istringstream istream(chunk);
	string line;
	getline(istream, line, '\n');
	while (getline(istream, line, '\n'))
	{
		if (line == "\r")
			break;
		p = parsePair(line);
		if (!p.first.empty())
			headers[p.first] = p.second;
	}
	parseCookies(headers["Cookie"]);
	if (ContainsMultipartHeader())
	{
		setBoundary();
		McontentLength = atoi(headers["Content-Length"].c_str());
	}
}

bool	Request::checkRequestSize()
{
	if ((_headers.find("Content-Length") != _headers.end() && atoi(_headers["Content-Length"].c_str()) > MAX_BODY_SIZE)
		|| _request.size() > MAX_BODY_SIZE)
	{
		_sizeInRange = false;
		return _sizeInRange;
	}
	return _sizeInRange;
}


bool	Request::ContainsMultipartHeader()
{
	if (_headers["Content-Type"].find("boundary=") != string::npos)
		return true;
	return false;
}

bool	Request::setBoundary()
{
	boundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("boundary="));
	boundary = boundary.substr(boundary.find_first_of('=') + 1);
	return true;
}

bool	Request::ApproveMultipart(const string preBoundary, const string bound)
{
	string expectedBoundary = preBoundary + this->boundary;
	if (bound == expectedBoundary)
	{
		MultipartApproved = 1;
		return true;
	}
	return false;
}

bool	Request::handleMultipart()
{
	Headers headers;
	if (Request::boundary.empty())
		return false;
	if (_standardRequest)
	{
		MultipartURI = _URI;
		if (_body.length() == 0)
			return true;
		parseMultipart(_body);
	}
	else
	{
		_URI = MultipartURI;
		parseMultipart(_request);
	}
	return true;
}

bool	Request::parseMultipart(const string & chunk)
{
	cout << RED << "parse Multipart" << NORM << endl;
	istringstream s(chunk);
	string body;
	string line;
	getline(s, line, '\n');
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	if (!MultipartApproved)
	{
		if (!ApproveMultipart("--", line))
		{
			_method = "POST";
			clearMultipartData();
			return (MultipartApproved = -1, false);
		}
		parseHeaders(_headers, chunk);
		setFilename();
		getline(s, line, '\n');
		parseBody(body, chunk, McontentLength - findDoubleNewline(_request));
	}
	else
		body = chunk;
	MultipartBody.append(body);
	packTogether();
	return true;
}

void Request::packTogether()
{
	if (MultipartBody.find(setBoundaryTogether(boundary, "end")) != string::npos)
	{
		MultipartBody.erase(MultipartBody.find(setBoundaryTogether(boundary, "end")));
		if (MultipartBody == "\r\n")
			cout << "File body is empty" << endl;
		_standardRequest = true;
		_method = "POST";
		_filename = MultipartName;
		_body = MultipartBody;
		_contentType = MultipartContentType;
		_URI = MultipartURI;
		clearMultipartData();
	}
}

void Request::setName(const string & attr, Headers & attributes)
{
	if (attributes.find(attr) != attributes.end())
	{
		if (!attributes[attr].empty())
		{
			MultipartName = attributes[attr];
			return;
		}
	}
}

void Request::setFilename()
{
	if (_headers["Content-Disposition"].empty())
		return ;
	istringstream is(_headers["Content-Disposition"]);
	string s;
	Headers attributes;
	getline(is, s, ';');
	while (getline(is, s, ';'))
		parseContentAttributes(attributes, s);
	setName("filename", attributes);
	if (MultipartName == ".empty.")
		setName("name", attributes);
}

void	Request::parseContentAttributes(stringMAP & attributes, const string & s)
{
	PAIR p;
	if (s.find('='))
	{
		p.first = s.substr(0, s.find('='));
		p.second = s.substr(s.find('=') + 1);
		p.second.erase(remove(p.second.begin(), p.second.end(), '"'), p.second.end());
		attributes[p.first] = p.second;
	}
}

void	Request::clearMultipartData()
{
	MultipartApproved = 0;
	MultipartBody = "";
	McontentLength = 0;
	boundary = "";
	MultipartContentType = "";
	MultipartName = ".empty.";
	MultipartURI = "";
}

const string Request::setBoundaryTogether(const string & bound, const string & type)
{
	string b = "--";
	b.append(bound);

	if (!type.empty() && type == "end")
		b.append("--");
	return b;
}
int Request::parseBody(string & body, const string & chunk, const int & length)
{
	int pos_body = findDoubleNewline(chunk);
	if (pos_body < 0)
		cerr << "No Body in Post request!" << endl;
	body = chunk.substr(pos_body, length);
	return 1;
}

void Request::parseQuery(const string & queryString)
{
	if (queryString.empty())
		return ;
	this->_envCgi["query"] = "QUERY_STRING=" + queryString;
	istringstream queryStream(queryString);
	string queryPair;
	while (getline(queryStream, queryPair, '&'))
	{
		size_t a = queryPair.find_first_of('=');
		if (a == string::npos || a == string::npos - 1)
			return ;
		_query[queryPair.substr(0, a)] = queryPair.substr(a + 1);
	}
	// Query::iterator it;
	// for (it = _query.begin(); it != _query.end(); it++)
	// 	cout << it->first << " and " << it->second << endl;

}

PAIR Request::parsePair(string line)
{
	PAIR p;

	line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	if (line.empty())
		return (p);
	istringstream iistream(line);
	getline(iistream, p.first, ':');
	getline(iistream, p.second);
	return p;
}

int Request::findDoubleNewline(const std::string &s)
{
	int pos = s.find("\r\n\r\n");
	if (pos > 0)
		pos += 4;
	else
	{
		pos = s.find("\n\n");
		if (pos > 0)
			pos += 2;
		else
			return -1;
	}
	return pos;
}

ostream & operator<<(ostream & os, const Request & r)
{
	if (r._standardRequest)
	{
		cout << "############################" << endl;
		cout << "Method: " << r._method << endl;
		cout << "Path: " << r._URI << endl;
		cout << "httpVersion: " << r._httpVersion << endl;
		cout << "##################################" << endl;
	}

	cout << "----------------HEADERS:---------------" << endl;
	map<string, string>::const_iterator it;
	for (it = (r._headers).begin(); it != (r._headers).end(); it++)
		cout << it->first << ":" << it->second << endl;
	cout << "------------------------------------------" << endl;

	cout << "///////////////BODY/////////////" << endl;
	if (r._body.length() > 0)
		cout << r._body << endl;
	else
		cout << "NO CONTENT" << endl;
	cout << "////////////////////////////////" << endl;
	return os;
}

void printMAP(stringMAP m)
{
	stringMAP::iterator it = m.begin();
	for (; it != m.end(); it++)
		cout << it->first << " , " << it->second << endl;
	return ;
}