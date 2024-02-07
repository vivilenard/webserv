#include "../../include/httpTransfer/Response.hpp"

Config Response::_config = Config();

Response::Response(Request & request):
	_request(request), _status("418 I'm a teapot"), _httpVersion("HTTP/1.1")
{
	cout << "created Response" << endl;
	if (request.getSizeBound() == false)
		{ formResponse("400 Bad Request"); return; }
	if (request.getMethod() == "GET")
		processGet();
	else if (request.getMethod() == "POST")
		processPost();
	else if (request.getMethod() == "DELETE")
		processDelete();
	else if (request.getMethod() == "HEAD")
		processGet();
	else 
		formResponse("501 Not Implemented");
}


void	Response::processGet()
{
	_status = "200 OK";
	ostringstream os;
	string	path = addRootPath(_request.getPath());
	if (path == _config.getRootPath() + "/")
		path = path + "index.html";
	cout << "path: " << path << endl;
	string	fileContent = readFile(path);
	if (fileContent.empty())
	{
		cerr << "file has no content" << endl;
		_status = "404 Not Found";
	}
	string	contentType = getContentType(path);
	if (contentType.empty())
		_status = "415 Unsupported Media Type";
	// cout << path << ", " << mimeType << endl;
	if (_status == "200 OK")
	{
		os << _httpVersion << " " << _status << "\r\n";
		os << "Content-Type:" 		<< contentType << "\r\n";
		os << "Content-Length: " 	<< fileContent.length() << "\r\n";
		os << "\r\n";
		if (_request.getMethod() != "HEAD")
			os << fileContent;
		_response = os.str();
		return ;
	}
	formResponse(_status);
}

void Response::formResponse(const string & status)
{
	ostringstream os;
	os << "<h1> " << status << " </h1>\n";
	// if (statusInfo.empty() == 0)
		// os << "<h3> " << statusInfo << " </h3>\n";
	// os.str("");
	string body = os.str();
	os.str("");

	// ostringstream os;
	os << _httpVersion << " " << status << "\r\n";
	if (status != "100 Continue")
	{
		os << "Content-Type:" 		<< "text/html" << "\r\n";
		os << "Content-Length: " 	<< body.length() << "\r\n";
		os << "\r\n";
		os << body;
	}
	_response = os.str();
	cout << "_________________RESPONSE________________" << endl;
	cout << _response << endl;
	cout << "_________________________________________" << endl;

}

void Response::processPost()
{
	cout << "-----------------IN POST--------------------" << endl;
	//IMPRO:
	if (isMultipart())
	{
		formResponse("100 Continue");
		return;
	}
	_status = "201 Created";
	string path = addRootPath(_request.getPath());
	string contentType = _request.getHeaders()["Content-Type"];
	string body = _request.getBody();
	string mimeType = findKeyByValue(_config._mimeTypes, contentType);
	cout << "MIME:" << mimeType << endl;
	if (mimeType == "form-urlencoded")
		_request.parseQuery(_request.getBody());
	else if (createFile(path, body) == 0)
		cerr << "failed to create file" << endl;
	formResponse(_status);
}

int	Response::createFile(std::string & path, std::string & content)
{
	fstream file;
	cout << "PATH:" << path << "!" << endl;
	if (_request.getPath() == "/")
	{
		_status = "400 Bad Request";
		_statusInfo = "Please include a valid path";
		return false;
	}
	if (access(path.c_str(), W_OK) > 0)
		cout << "file already exists and has writing rights, its going to be overwritten" << endl;
	cout << "TRY TO ACCESS: " << path << endl;
	file.open(path.c_str(), ios::trunc | ios::binary | ios::out);
	if (!file.is_open())
	{
		_status = "400 Bad Request";
		_statusInfo = "Please include a valid path";
		return false;
	}
	file << content;
	file.close();
	return true;
}

void Response::processDelete()
{
	cout << "DELETE REQUEST" << endl;
	string path = addRootPath(_request.getPath());
	if (std::remove(path.c_str()) == 0)
		_status = "200 OK";
	else 
		_status = "404 Not Found";
	formResponse(_status);
}

bool	Response::isMultipart()
{
	if (_request.getHeaders()["Content-Type"].find("boundary=") != string::npos)
		return true;
	return false;
}
string	Response::readFile(const string & path)
{
	ifstream		file;
	string			fileContent;
	ostringstream	os;

	file.open(path.c_str());
	if (!file)
		{
			cerr << "File does not open, probably no access rights" << endl;
			return "";
		}
	os << file.rdbuf();
	fileContent = os.str();

	os.str(""); file.close();
	return fileContent;
}

string	Response::addRootPath(const string & path)
{
	// string path = _request.getPath();
	//add prefix
	return (_config.getRootPath() + path);

}

string Response::getContentType(const string & path)
{
	int j = path.find_last_of(".");
	if (j == -1)
		return "";
	string mime = path.substr(j + 1);
	string contentType = _config._mimeTypes[mime];
	return contentType;
}

std::string	Response::findKeyByValue(std::map<string, string>m, string value)
{
	std::string fileType;
	map<string, string>::iterator it;
	for (it = m.begin(); it != m.end(); it++)
	{
		// cout << it->second << "," << contentType << it->second.compare(contentType) << endl;
		if (it->second.compare(value) == 0)
		{
			cout << it->first << "," << it->second << endl;
			fileType = it->first;
			break;
		}
	}
	return fileType;
}