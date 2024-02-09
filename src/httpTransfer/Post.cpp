#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processPost()
{
	cout << "-----------------IN POST--------------------" << endl;
	if (isCgi(_URI))
		return ;
	if (isMultipart()){
		formResponse(100, _statusInfo);
		return; }

	_status = 201;
	string contentType = _request.getHeaders()["Content-Type"];
	string mimeType = findKeyByValue(_config._mimeTypes, contentType);
	// cout << "MIME:" << mimeType << endl;
	if (mimeType == "form-urlencoded")
		_request.parseQuery(_request.getBody());
	else if (createFile(_URI, _request.getBody()) == 0)
		cerr << "failed to create file" << endl;
	formResponse(_status, _statusInfo);
}

int	Response::createFile(std::string & path, const std::string & content)
{
	fstream file;
	// cout << "PATH:" << path << "!" << endl;
	if (_request.getURI() == "/")
	{
		_status = 400; _statusInfo = "Please include a valid path";
		return false;
	}
	if (access(path.c_str(), W_OK) > 0)
		cout << "file already exists and has writing rights, its going to be overwritten" << endl;
	// cout << "TRY TO ACCESS: " << path << endl;
	file.open(path.c_str(), ios::trunc | ios::binary | ios::out);
	if (!file.is_open())
	{
		_status = 400; _statusInfo = "Please include a valid path";
		return false;
	}
	file << content;
	file.close();
	return true;
}

bool	Response::isMultipart()
{
	if (_request.getHeaders()["Content-Type"].find("boundary=") != string::npos)
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
			cout << it->first << "," << it->second << endl;
			fileType = it->first;
			break;
		}
	}
	return fileType;
}

