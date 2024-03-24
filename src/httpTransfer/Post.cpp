#include "Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processPost()
{
	cout << "-----------------IN POST--------------------" << endl;
	cout << NORM << "URI: " << _URI << endl;
	_status = 201;
	string contentType = findKeyByValue(_configfile._mimeTypes, _request.getHeaders()["Content-Type"]);
	if (isCgi(_URI))
		{ formResponse(201, _statusInfo); return ; }
	if (isMultipart())
		{ formResponse(100, _statusInfo); return; }
	if (recieveQuery(contentType)) 
	{
		handleQuery();
		formResponse(201, "recieved Data");
		return ;
	}
	if (incorrectMimeType(_request.getHeaders()["Content-Type"]))
		{ formResponse(400, "Incorrect Mimetype"); }
	else if (!createFile(_URI, _request.getBody()))
		{ _status = 400; _statusInfo = "Please include a valid path. File could not be created"; }
	formResponse(_status, "");
}

bool Response::incorrectMimeType(const string & contentType)
{
	string filepath = _URI;
	if (!_request.getFilename().empty())
		filepath = _request.getFilename();
	size_t mimePos = filepath.find_last_of('.');
	string mime = "";
	if (mimePos != string::npos)
		mime = filepath.substr(mimePos + 1);
	if (mime.empty() && !(contentType == "text/plain" || contentType == "plain/text"))
		return true;
	if (_configfile._mimeTypes[mime] != contentType && !(contentType == "text/plain" || contentType == "plain/text"))
	{
		cout << _configfile._mimeTypes[mime] << endl;
		cout << contentType << endl;
		return true;
	}
	return false;
}

int Response::handleMultipart()
{
	formResponse(100, _statusInfo);
	if (Request::MultipartApproved == -1)
	{
		cout << "Multipart not Approved" << endl;
		formResponse(400, "Incorrect Boundary");
		Request::MultipartApproved = 0;
		return -1;
	}
	return 1;
}

int	Response::createFile(std::string & path, const std::string & content)
{
	fstream file;
	string filepath = path;
	if (!_request.getFilename().empty())
		filepath = _configfile._root + "/upload" + "/" + _request.getFilename();
	file.open(filepath.c_str(), ios::trunc | ios::binary | ios::out);
	if (!file.is_open())
	{
		cout << "File: " << filepath.c_str() << " could not be created." << endl;
		return false;
	}
	file << content;
	file.close();
	return true;
}

int	Response::isMultipart()
{
	if (!_request.boundary.empty())
	{
		cout << "isMultipart" << endl;
		return 1;
	}
	return 0;
}

bool Response::noFilename()
{
	if (!_request.getFilename().empty())
	{
		if (_request.getFilename() == ".empty.")
		{
			cout << "Multipart not approved, no filename" << endl;
			return 1;
		}
	}
	return 0;
}

std::string	Response::findKeyByValue(std::map<string, string>m, string value)
{
	std::string fileType;
	map<string, string>::iterator it;
	for (it = m.begin(); it != m.end(); it++)
	{
		if (it->second.compare(value) == 0)
		{
			fileType = it->first;
			break;
		}
	}
	return fileType;
}

bool			Response::recieveQuery(const string & contentType)
{
	if (contentType == "form-urlencoded")
	{
		_request.parseQuery(_request.getBody());
		Query query = _request.getQuery();
		Query::iterator it = query.begin();
		for (; it != query.end(); it++)
		{
			cout << it->first << " " << it->second << endl;
		}
		return true;
	}
	return false;
}

void	Response::handleQuery()
{
	if (_request.getURI() != "/login")
		return ;
	bakeLoginCookie(_request.getQuery());
}
