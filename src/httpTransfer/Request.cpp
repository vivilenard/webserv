#include "../../include/httpTransfer/Request.hpp"

// Config Request::_config = Config();

Request::Request(const string & request): _request(request), _sizeInRange(true)
{
	// cout << "------------REQUEST------------" << endl;
	// cout << request << endl;
	// cout << "-------------------------------" << endl;
	if (request.length() > MAX_BODY_SIZE + 500)
		_sizeInRange = false;
	parseMainHeader();
	parseHeaders();
	parseBody();
}

void	Request::parseMainHeader()
{
	// cout << "parse Header" << endl;
	istringstream istream(_request);
	if (!getline(istream, _method, ' ')
		|| !getline(istream, _path, ' ')
		|| !getline(istream, _httpVersion, ' '))
		cerr << "Error: Wrong Header" << endl;

	_httpVersion = _httpVersion.substr(0, _httpVersion.find_first_of("\n"));
	size_t pos = _path.find_first_of('?');
	if (pos != string::npos)
	{
		parseQuery(_path.substr(pos + 1));
		_path.erase(pos);
		cout << "path: " << _path << endl ;
	}
}

void	Request::parseHeaders()
{
	istringstream istream(_request);
	string line;
	getline(istream, line, '\n');
	while (getline(istream, line))
	{
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
}


int Request::parseBody()
{
	//PROBLEM: BUFFER ONLY 42185 BYTES
	int pos_body = findDoubleNewline(_request);
	if (pos_body < 0)
		cerr << "No Body in Post request!" << endl;
	istringstream is(_headers["Content-Length"]);
	int contentLength;
	is >> contentLength;
	cout << "body pos: " << pos_body << endl;
	cout << "contentLength: " << contentLength << endl;
	_body = _request.substr(pos_body, contentLength);
	cout << "BODY SIZE: " << _body.length() << endl;
	cout << "REQUEST SIZE: " << _request.length() << endl;
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
	cout << "Path: " << r._path << endl;
	cout << "httpVersion: " << r._httpVersion << endl;
	cout << "##################################" << endl;

	cout << "----------------HEADERS:---------------" << endl;
	map<string, string>::const_iterator it;
	for (it = (r._headers).begin(); it != (r._headers).end(); it++)
		cout << it->first << ":" << it->second << endl;
	cout << "------------------------------------------" << endl;

	// cout << "///////////////BODY/////////////" << endl;
	// if (r._body.length() > 0)
	// 	cout << r._body << endl;
	// else
	// 	cout << "NO CONTENT" << endl;
	// cout << "////////////////////////////////" << endl;
	return os;
}

// void Request::printHeaders()
// {
// 	cout << "----------------PARSEHEADERS:---------------" << endl;
// 	Headers::iterator it;
// 	for (it = _headers.begin(); it != _headers.end(); it++)
// 		cout << it->first << ":" << it->second << endl;
// 	cout << "------------------------------------------" << endl;
// }

// string	Request::parseHeader(string s)
// {
// 	size_t pos_type = _request.find(s);
// 	//cout << "POS TYPE"<< pos_type << endl;
// 	if (pos_type == string::npos)
// 	{
// 		cerr << "No Content-Type found" << endl;
// 		return "";
// 	}
// 	std::string typeLine = _request.substr(pos_type, _request.find('\n', pos_type));
// 	typeLine = typeLine.substr(typeLine.find_first_of(':') + 1);
// 	typeLine = typeLine.substr(typeLine.find_first_not_of(' '));
// 	int pos = typeLine.find_first_of('\n');
// 	if ((int)typeLine.find_first_of('\r') < pos)
// 		pos = typeLine.find_first_of('\r');
// 	std::string contentType = typeLine.substr(0, pos);
// 	return contentType;
// }