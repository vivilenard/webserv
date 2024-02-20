#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processPost()
{
	cout << "-----------------IN POST--------------------" << endl;
	cout << NORM << "URI: " << _URI << endl;
	if (isCgi(_URI))
		return ;
	if (isMultipart())
	{
		formResponse(100, _statusInfo);
		return ;
	}
	//do multipart with MultipartName, body etc!
	_status = 201;
	string contentType = _request.getHeaders()["Content-Type"];
	string mimeType = findKeyByValue(_config._mimeTypes, contentType);
	if (mimeType == "form-urlencoded")
		_request.parseQuery(_request.getBody());
	else if (!createFile(_URI, _request.getBody()))
		{_status = 400; _statusInfo = "Please include a valid path";}
	formResponse(_status, _statusInfo);
}

int	Response::createFile(std::string & path, const std::string & content)
{
	fstream file;
	string filepath = path;
	if (!_request.getFilename().empty())
		filepath = _config.getRootPath() + "/upload" + "/" + _request.getFilename();
	// cout << GREEN << filepath << NORM << endl;
	file.open(filepath, ios::trunc | ios::binary | ios::out);
	if (!file.is_open())
		return false;
	file << content;
	file.close();
	return true;
}

bool	Response::isMultipart()
{
	if (!_request.boundary.empty())
		return true;
	return false;
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

