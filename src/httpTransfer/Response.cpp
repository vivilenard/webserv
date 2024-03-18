#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

StatusCode Response::_statusCode = StatusCode();

Response::Response(Request & request, configServer & configfile): _configfile(configfile),
	_request(request), _status(500), _statusInfo(""), _httpVersion("HTTP/1.1")
{
	// cout << "in Response " << endl;
	_URI = addRootPath(_request.getURI());
	cout << RED << _URI << NORM << endl;
	if (invalidRequest())
		return ;
	if (request.getMethod() == "GET" && methodAllowed())
		processGet();
	else if (request.getMethod() == "POST" && methodAllowed())
		processPost();
	else if (request.getMethod() == "DELETE" && methodAllowed())
		processDelete();
	else if (request.getMethod() == "HEAD" && methodAllowed())
		processGet();
	else 
		formResponse(_status, _statusInfo);
	cout << BLUE << STATUSCODE[_status] << endl << _statusInfo << NORM << endl;
}

bool Response::methodAllowed()
{
	bool methodAllowed = true;
	string method = _request.getMethod();
	string uri = _request.getURI();
	string directory = getDir(uri);
	// string uri = _URI;
	LOCATION locations = _configfile._locations;

	// cout << ORANGE << "method: " << method << NORM << endl;
	// cout << "uri: " << uri << endl;
	// cout << "directory: " << directory << NORM << endl;
	if (locations.find(directory) != locations.end())
	{
		// cout << "location: " << ORANGE << locations[directory]._name << " " << locations[directory]._get << NORM << endl;
		if (method == "GET")
		{
			// cout << "GET METHOD" << endl;
			if (locations[directory]._get == false)
				methodAllowed = false;
				// return (cout << RED << method << ": not allowed for uri: " << uri << NORM << endl, false);
		}
		else if (method == "POST")
		{
			// cout << "POST METHOD" << endl;
			if (locations[directory]._post == false)
				methodAllowed = false;
				// return (cout << RED << method << ": not allowed for uri: " << uri << NORM << endl, false);
		}
		else if (method == "DELETE")
		{
			// cout << "DELETE METHOD" << endl;
			if (locations[directory]._delete == false)
				methodAllowed = false;
				// return (cout << RED << method << ": not allowed for uri: " << uri << NORM << endl, false);
		}
	}
	if (methodAllowed == false)
	{
		_status = 405;
		cout << RED << method << ": not allowed for uri: " << uri << NORM << endl;
		return false;
	}
	return true;
}
bool Response::invalidRequest()
{
	if (_request.getSizeInRange() == false)
	{
		formResponse(400, "Request size is too large");
		return true;
	}
	return false;
}

void Response::formResponse(const int & status, const string & statusInfo)
{
	_status = status;
	_statusInfo = statusInfo;
	string body;

	if (_cgiScript.empty() == false)
	{
		body = _cgiScript;
	}
	else if (status == 200)
		body = _fileContent;
	else if(status != 200)
		body = createErrorBody(status, statusInfo);

	ostringstream os;
	os << _httpVersion << " " << STATUSCODE[status] << "\r\n";
	if (status != 100)
	{
		os << "Content-Type:" 		<< _fileContentType << "\r\n";
		os << "Content-Length: " 	<< body.length() << "\r\n";
		os << "\r\n";
		os << body;
	}
	_response = os.str();
	// cout << ORANGE << _status << NORM << endl;
}

const string Response::createErrorBody(const int & status, const string & statusInfo)
{
	ostringstream body_os;
	_fileContentType = "text/html";

	body_os << "<h3> " << STATUSCODE[status] << " </h3>\n";
	if (!statusInfo.empty())
		body_os << "<h3> " << statusInfo << "</h3>";
	return body_os.str();
}


//-------------------Utils-------------------

string	Response::addRootPath(const string & path)
{
	return (_configfile._root + path);
}

const string getDir(string & uri)
{
	//    /files/upload.html
	string dir = uri;
	if (!uri.find('.')) //endpoint is already a directory
		return uri;
	if (uri.find_last_of("/") < uri.npos - 1)
		dir = uri.substr(0, uri.find_last_of("/"));
	return dir;
}

//if there uri was '/', then add default file (index)
int	Response::addDefaultFile(string & path)
{
	if (path == _configfile._root + "/")
	{
		path = path.append(_configfile._index);			//change to defaultFile
		return 1;
	}
	return 0;
}
