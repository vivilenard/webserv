#include "../../include/httpTransfer/Request.hpp"

string Request::boundary = "bound";

Request::Request(const string & request): _request(request), _sizeInRange(true)
{
	// cout << "------------REQUEST------------" << endl;
	// cout << request << endl;
	// cout << "-------------------------------" << endl;
	if (request.length() > MAX_BODY_SIZE + 500)
		_sizeInRange = false;
	parseMainHeader();
	if (_standardRequest)
	{
		parseHeaders();
		parseBody();
	}
	handleMultipart();
	cout << "standardRequest: " << _standardRequest << endl;
	cout << "Boundary: " << boundary << "$" << endl;
}

void	Request::parseMainHeader()
{
	// cout << "parse Header" << endl;
	istringstream istream(_request);
	getline(istream, _method, ' ');
	getline(istream, _URI, ' ');
	getline(istream, _httpVersion, ' ');
	if (_httpVersion.find("HTTP/") != string::npos)
	{
		_standardRequest = true;
		boundary = "";
	}
	else
	{ _standardRequest = false; return; }

	_httpVersion = _httpVersion.substr(0, _httpVersion.find_first_of("\n"));
	size_t pos = _URI.find_first_of('?');
	if (pos != string::npos)
	{
		parseQuery(_URI.substr(pos + 1));
		_URI.erase(pos);
		// cout << "path: " << _URI << endl ;
	}
}

void	Request::parseHeaders()
{
	handleMultipart();
	istringstream istream(_request);
	string line;
	getline(istream, line, '\n');
	while (getline(istream, line))
	{
		// parsePair(p);
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
		if (line.empty()) break;
		istringstream iistream(line);
		pair<string, string> p;
		getline(iistream, p.first, ':');
		getline(iistream, p.second);
		_headers.insert(p);
		iistream.str("");
	}
	if (ContainsMultipartHeader())
		setBoundary();
}

bool	Request::ContainsMultipartHeader()
{
	if (_headers["Content-Type"].find("boundary=") != string::npos)
		return true;
	return false;
}

bool	Request::isMultipartChunk()
{
	string preBoundary = "--";
	string expectedBoundary = preBoundary.append(boundary);
	istringstream istream(_request);
	string firstLine;
	getline(istream, firstLine, '\r');
	cout << expectedBoundary << endl;
	if (firstLine == expectedBoundary)
	{
		cout << "its a correct boundaryyyyyyy" << endl;
		return true;
	}
	return false;
}

bool	Request::setBoundary()
{
	cout << "Boundary at:" << _headers["Content-Type"].find("boundary=") << endl;
	boundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("boundary="));
	return true;
}

bool	Request::handleMultipart()
{
	if (Request::boundary.empty())
		return false;
	return true;
	string chunk;
	if (_standardRequest)
		chunk = _body;
	else if (isMultipartChunk())
		chunk = _request;
	//remove first line

	//read Headers

	//read Body >> static body

	//if last line is boundary: remove and set multipart as finished
}

int Request::parseBody()
{
	int pos_body = findDoubleNewline(_request);
	if (pos_body < 0)
		cerr << "No Body in Post request!" << endl;
	istringstream is(_headers["Content-Length"]);
	int contentLength;
	is >> contentLength;
	_body = _request.substr(pos_body, contentLength);
	return 1;
}

void Request::parseQuery(const string & queryString)
{
	if (queryString.empty())
		return ;
	cout << queryString << endl;
	istringstream queryStream(queryString);
	string queryPair;
	while (getline(queryStream, queryPair, '&'))
	{
		size_t a = queryPair.find_first_of('=');
		if (a == string::npos || a == string::npos - 1)
			return ;
		_query[queryPair.substr(0, a)] = queryPair.substr(a + 1);
	}
	Query::iterator it;
	for (it = _query.begin(); it != _query.end(); it++)
		cout << it->first << " and " << it->second << endl;

}

int Request::findDoubleNewline(std::string &s)
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
	cout << "############################" << endl;
	// cout << "Request: " << r._request << endl;
	cout << "Method: " << r._method << endl;
	cout << "Path: " << r._URI << endl;
	cout << "httpVersion: " << r._httpVersion << endl;
	cout << "##################################" << endl;

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
