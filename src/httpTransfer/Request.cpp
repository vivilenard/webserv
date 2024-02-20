#include "../../include/httpTransfer/Request.hpp"

bool	Request::MultipartApproved = false;
string	Request::boundary = "bound";
string	Request::MultipartBody = "";
int		Request::McontentLength = 0;
string	Request::MultipartContentType = "text/plain";
string	Request::MultipartName = "file.txt";

Request::Request(const string & request): _request(request), _filename(""), _sizeInRange(true)
{
	// cout << "------------REQUEST------------" << endl;
	// cout << request << endl;
	// cout << "-------------------------------" << endl;
	cout << "LENGTH: " << request.length() << endl;
	if (request.length() > MAX_BODY_SIZE + 500)
		_sizeInRange = false;
	parseMainHeader();
	if (_standardRequest)
	{
		parseHeaders(_headers);
		parseBody(_body, _request, atoi(_headers["Content-Length"].c_str()));
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
	_httpVersion = _httpVersion.substr(0, _httpVersion.find_first_of("\n"));
	size_t pos = _URI.find_first_of('?');
	if (pos != string::npos)
	{
		parseQuery(_URI.substr(pos + 1));
		_URI.erase(pos);
		// cout << "path: " << _URI << endl ;
	}
	identifyRequest();
}


void Request::identifyRequest()
{
	if (_httpVersion.find("HTTP/") != string::npos)
	{
		_standardRequest = true;
		clearMultipartData();
		return ;
	}
	_standardRequest = false;
}

void	Request::parseHeaders(Headers & headers)
{
	PAIR p;
	istringstream istream(_request);
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
	if (ContainsMultipartHeader())
	{
		setBoundary();
		McontentLength = atoi(headers["Content-Length"].c_str());
	}
}

bool	Request::ContainsMultipartHeader()
{
	if (_headers["Content-Type"].find("boundary=") != string::npos)
		return true;
	return false;
}

bool	Request::isMultipartChunk()
{
	istringstream istream(_request);
	string firstLine;
	getline(istream, firstLine, '\r');
	if (ApproveMultipart("--", firstLine))
		return true;
	return false;
}

bool	Request::setBoundary()
{
	cout << "Boundary at:" << _headers["Content-Type"].find("boundary=") << endl;
	boundary = _headers["Content-Type"].substr(_headers["Content-Type"].find("boundary="));
	boundary = boundary.substr(boundary.find_first_of('=') + 1);
	return true;
}

bool	Request::ApproveMultipart(const string preBoundary, const string bound)
{
	// cout << "BOUND CHECK:" << endl;
	string expectedBoundary = preBoundary + this->boundary;
	cout << expectedBoundary << endl;
	if (bound == expectedBoundary)
	{
		cout << "its a correct boundaryyyyyyy" << endl;
		MultipartApproved = true;
		return true;
	}
	return false;
}

bool	Request::handleMultipart()
{
	cout << MAG << "HANDLE MULTIPART" << NORM << endl;
	Headers headers;
	if (Request::boundary.empty())
	{
		cout << "no boundary" << endl;
		return false;
	}
	if (_standardRequest)
		parseMultipart(_body); //shoult look if there is sth before boundary
	else// if (isMultipartChunk())
		parseMultipart(_request);
	
	return true;
}

bool	Request::parseMultipart(const string & chunk)
{
	cout << MAG << "parse Multipart" << NORM << endl;
	cout << MAG << chunk << NORM << endl;
	//remove first line
	istringstream s(chunk);
	string body;
	string line;
	getline(s, line, '\n');
	line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
	if (!MultipartApproved)
	{
		if (!ApproveMultipart("--", line))
			return (cout << "boundary not equal" << endl, false);
		parseHeaders(_headers); //adds headers to existing ones
		setFilename();
		cout << "filename: " << MultipartName <<endl;
		getline(s, line, '\n'); //should be empty line
		parseBody(body, chunk, McontentLength - findDoubleNewline(_request));
	}
	else
		body = chunk;
	MultipartBody.append(body);
	//if last line is boundary: remove and set multipart as finished
	if (MultipartBody.find(setBoundaryTogether(boundary, "end")) != string::npos)
	{
		cout << RED << "FOUND END BOUND" << NORM << endl;
		MultipartBody.erase(MultipartBody.find(setBoundaryTogether(boundary, "end")));
		_body = MultipartBody;
		_filename = MultipartName;
		_contentType = MultipartContentType;
		_method = "POST";
		_standardRequest = true;
		clearMultipartData();
	}
	cout << ORANGE << "MultipartBody: " << NORM << endl;
	cout << ORANGE << MultipartBody << NORM << endl;
	return true;
}

void Request::setFilename()
{
	cout << GREEN << "SETFILENAME" << NORM << endl;
	if (_headers["Content-Disposition"].empty())
		return ;
	istringstream is(_headers["Content-Disposition"]);
	string s;
	string FindHeader = "filename=";
	while (getline(is, s, ';'))
	{
		cout << s << endl;
		if (s.find(FindHeader) != string::npos)
		{
			s.erase(0, FindHeader.size());
			s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
			MultipartName = s;
			break ;
		}
	}
}

void	Request::clearMultipartData()
{
	MultipartApproved = false;
	MultipartBody = "";
	McontentLength = 0;
	boundary = "";
	MultipartContentType = "text/plain";
	MultipartName = "file.txt";
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
	// iistream.str("");
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
	// cout << "Request: " << r._request << endl;
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
	// else if (r.MultipartBody.length() > 0 && !r._standardRequest)
	// {
	// 	cout << r.MultipartBody << endl;
	// }
	else
		cout << "NO CONTENT" << endl;
	cout << "////////////////////////////////" << endl;
	return os;
}
