#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

Config Response::_config = Config();
StatusCode Response::_statusCode = StatusCode();

Response::Response(Request & request):
	_request(request), _status(500), _statusInfo(), _httpVersion("HTTP/1.1")
{
	// cout << GREEN << _request.getURI() << NORM << endl;
	_URI = addRootPath(_request.getURI());
	// cout << RED << _URI << NORM << endl;
	if (invalidRequest())
		return ;
	if (request.getMethod() == "GET")
		processGet();
	else if (request.getMethod() == "POST")
		processPost();
	else if (request.getMethod() == "DELETE")
		processDelete();
	else if (request.getMethod() == "HEAD")
		processGet();
	else 
		formResponse(501, _statusInfo);
	cout << BLUE << STATUSCODE[_status] << " " << _statusInfo << NORM << endl;
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
	if (status == 200)
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
	return (_config.getRootPath() + path);

}

//if there uri was '/', then add default file (index)
int	Response::addDefaultFile(string & path)
{
	if (path == _config.getRootPath() + "/")
	{
		path = path + "index.html";			//change to defaultFile
		return 1;
	}
	return 0;
}