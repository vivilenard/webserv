#include "../../include/httpTransfer/Response.hpp"
#include "../../include/httpTransfer/Request.hpp"

void	Response::processGet()
{
	_status = 200;
	ostringstream os;
	if (listDirectory(this->_URI))
		{ formResponse(200, ""); return ; }
	// addDefaultFile(_URI);
	if (isCgi(_URI))
		{ return ; }
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


bool	Response::beginsWithDot(const string & s)
{
	// if (s == "..") //exception
	// 	return false;
	if (s[0] == '.')
		return true;
	return false;
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

bool	Response::listDirectory(const string & endpoint)
{
	// cout << RED << "DIRECTORY LISTING" << NORM << endl;
	vector<string> v; ostringstream os;
	DIR* dirp = opendir(endpoint.c_str()); struct dirent * dp;
	if (!dirp)
		{ return (cout << "its not a directory" << endl, false); }
	os << "<h1> Index of " << currentDir() << "</h1><br>" << endl;
	while ((dp = readdir(dirp)) != NULL) {
		// v.push_back(dp->d_name);
		if (beginsWithDot(dp->d_name)) continue ;
		string newPath = currentDir().append(dp->d_name);
		os << "<a href=" << "\"" << newPath << "\">" << dp->d_name // << getFileCreationTime((char *)newPath.c_str())
			<< "</a>" << endl;
		os << "<br>" << endl;
	}
	closedir(dirp);
	_fileContent = os.str();
	return true;
}

string Response::currentDir()
{
	string curDir = "/";
	if (_request.getURI() == "/")
		return curDir;
	curDir = _request.getURI() + "/";
	return curDir;
}

string Response::getFileCreationTime(char *path) {
	struct stat attr;
	stat(path, &attr);
	printf("Last modified time: %s", ctime(&attr.st_mtime));
	return ctime(&attr.st_mtime);
}