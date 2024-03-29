#include "Response.hpp"

#define STATUSCODE _statusCode.getStati()

StatusCode Response::_statusCode = StatusCode();

Response::Response(Request & request, configServer & configfile): _configfile(configfile),
	_request(request), _status(501), _redir(0), _statusInfo(""), _httpVersion("HTTP/1.1")
{
	if (PRINT) { cout << "---------Response---------" << endl; }
	_URI = redirectURI(_request.getURI());
	_URI = addRootPath(_URI);
	if (PRINT) {cout << "Response: " << BLUE << _URI << NORM << endl;}
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
		formResponse(_status, "");
}

bool Response::methodAllowed()
{
	bool methodAllowed = true;
	string method = _request.getMethod();
	string uri = _request.getURI();
	string directory = getDir(uri);
	LOCATION locations = _configfile._locations;


	if (locations.find(directory) != locations.end())
	{
		if (method == "GET")
		{
			if (locations[directory]._get == false)
				methodAllowed = false;
		}
		else if (method == "POST")
		{
			if (locations[directory]._post == false)
				methodAllowed = false;
		}
		else if (method == "DELETE")
		{
			if (locations[directory]._delete == false)
				methodAllowed = false;
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

void Response::formResponse(int status, const string & statusInfo)
{
	_status = status;
	_statusInfo = statusInfo;
	string body;

	if (_cgiScript.empty() == false)
		body = _cgiScript;
	else if (status == 200)
		body = _fileContent;
	else if(status != 200)
		body = createErrorBody(status, statusInfo);

	ostringstream os;
	if (status == 200 && (_redir >= 300 && _redir < 400))
		status = _redir;
	os << _httpVersion << " " << STATUSCODE[status] << "\r\n";
	if (status != 100)
	{
		os << "Content-Type: " 		<< _fileContentType << "\r\n";
		os << "Content-Length: " 	<< body.length() << "\r\n";
		os << CookiesToHeaders();
		os << "\r\n";
		os << body;
	}
	_response = os.str();
}


const string FileToString(const string & filepath)
{
	string str = "";
	ifstream file(filepath.c_str());
	if (file)
	{
		stringstream buffer;
		buffer << file.rdbuf();
		str = buffer.str();
	}
	file.close();
	return str;
}

const string Response::generateDefaultHtml(const string & text)
{
	string outputString = FileToString("error/default1of2.html") + text;
	outputString.append(FileToString("error/default2of2.html"));
	return outputString;
}

const string Response::createErrorBody(const int & status, const string & statusInfo)
{
	_fileContentType = "text/html";
	ostringstream body_os;

	if (status == 7)
		return (FileToString("error/defaultDirectory.html"));
	else if (status == 404) //&& _request.getURI() == "/dog")
		return (FileToString("error/404.html"));
	else if (status == 400 && statusInfo.empty())
		return (FileToString("error/400.html"));
	else if (_request.getURI() == "/login" && status == 201)
	{
		string cookieBody = getCookieBody();
		if (!cookieBody.empty())
		{
			body_os << "<h3> " << cookieBody << "</h3>";
			return (generateDefaultHtml(body_os.str()));
		}
	}
	body_os << "<h3> " << STATUSCODE[status] << " </h3>\n";
	if (status == 404)
		body_os << "<h3> Wrong path: " << _request.getURI() << "</h3>";
	else if (!statusInfo.empty())
		body_os << "<h3> " << statusInfo << "</h3>";
	return generateDefaultHtml(body_os.str());
}


//-------------------Utils-------------------
string	Response::addPaths(string p1, string p2)
{
	if (p1[p1.size() - 1] == '/' && p2[0] == '/')
	{
		p1 = p1.substr(0, p1.size() - 1);
	}
	return (p1 + p2);
}

string	Response::addRootPath(const string & path)
{
	return (addPaths(_configfile._root, path));
}

string	Response::redirectURI(string path)
{
	if (_redir > 0)
		return path;
	LOCATION locations = _configfile._locations;
	if (locations.find(path) != locations.end() && !locations[path]._redirect.empty())
	{
		path = locations[path]._redirect;
		_redir = 308;
		cout << path << endl;
	}
	return path;
}

const string getDir(string & uri)
{
	string dir = uri;
	if (!uri.find('.'))
		return uri;
	if (uri.find_last_of("/") < uri.npos - 1)
		dir = uri.substr(0, uri.find_last_of("/"));
	return dir;
}

int	Response::addDefaultFile(string & path)
{
	if (path == _configfile._root + "/")
	{
		path = path.append(_configfile._index);
		return 1;
	}
	return 0;
}
