#include "../../include/httpTransfer/Response.hpp"

void	Response::processGet()
{
	_status = 200;
	ostringstream os;
	addDefaultFile(_URI);
	if (isCgi(_URI))
		return ;
	if (!readFile(_URI))
	{
		_status = 404;
		_statusInfo = "Not Found";
	}
	_fileContentType = getContentType(_URI);
	if (_fileContentType.empty())
		_status = 415;
	formResponse(_status, "");
}

bool	Response::readFile(const string & path)
{
	ifstream		file;
	string			fileContent;
	ostringstream	os;

	file.open(path.c_str());
	if (!file)
		{
			cerr << "File does not open, probably no access rights" << endl;
			return false;
		}
	os << file.rdbuf();
	_fileContent = os.str();

	os.str(""); file.close();
	return true;
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