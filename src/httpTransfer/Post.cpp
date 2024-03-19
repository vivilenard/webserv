#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processPost()
{
	cout << "-----------------IN POST--------------------" << endl;
	cout << NORM << "URI: " << _URI << endl;
	// cout << "filename: " << _request.getFilename() << endl;
	_status = 201;
	if (isCgi(_URI)) // if is error we got throw an error
	{
		formResponse(201, _statusInfo);
		return ;
	}
	if (isMultipart()){
		formResponse(100, _statusInfo);
		return; }
	string contentType = findKeyByValue(_configfile._mimeTypes, _request.getHeaders()["Content-Type"]);
	if (contentType == "form-urlencoded")
		{_request.parseQuery(_request.getBody()); return; }
	if (incorrectMimeType(_request.getHeaders()["Content-Type"]))
		{ formResponse(400, "Incorrect Mimetype");
		cout << ORANGE << "JOO: " << incorrectMimeType(_request.getHeaders()["Content-Type"]) << endl; return; }
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
		cout << "doesnt match: " << endl;
		cout << _configfile._mimeTypes[mime] << endl;
		cout << contentType << endl;
		return true;
	}
	return false;
}

int Response::handleMultipart()
{
	cout << "handle Multipart" << endl;
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
	//config filepath for upload
	if (!_request.getFilename().empty())
		filepath = _configfile._root + "/upload" + "/" + _request.getFilename();
	// cout << GREEN << filepath << NORM << endl;
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
