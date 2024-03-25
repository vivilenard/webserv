#include "Response.hpp"
#include "Request.hpp"

void	Response::processGet()
{
	_status = 200;
	ostringstream os;
	addDefaultFile(_URI);
	if (isCgi(_URI))
		{ formResponse(_status, ""); return ; }
	_fileContentType = getContentType(_URI);
	if (isDirectory(this->_URI))
		{ formResponse(_status, ""); return ; }
	if (!readFile(_URI))
		{ formResponse(404, ""); return ; }
	if (_fileContentType.empty())
		{ formResponse(415, ""); return ; }
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
	const char* filepath = path.c_str();

	file.open(filepath);
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
	string contentType = _configfile._mimeTypes[mime];
	return contentType;
}

bool	Response::listDirectory(DIR* dirp)
{
	ostringstream os;
	struct dirent * dp;
	os << "<h1> Index of " << currentDir() << "</h1><br>" << endl;
	while ((dp = readdir(dirp)) != NULL) {
		if (beginsWithDot(dp->d_name)) continue ;
		string newPath = currentDir().append(dp->d_name);
		os << "<a href=" << "\"" << newPath << "\">" << dp->d_name // << getFileCreationTime((char *)newPath.c_str())
			<< "</a>" << endl;
		os << "<br>" << endl;
	}
	_fileContent = os.str();
	return true;
}

bool	Response::isDirectory(const string & endpoint)
{
	DIR* dirp = opendir(endpoint.c_str());
	if (!dirp)
		return false;
	_status = 200;
	if (_configfile._directoryListing == true)
		listDirectory(dirp);
	else
		_status = 7; //its a directory
	closedir(dirp);
	return true;
}

string Response::currentDir()
{
	string curDir = "/";
	if (_request.getURI() == "/")
		return curDir;
	curDir = addPaths(_request.getURI(),  "/");
	return curDir;
}

string Response::getFileCreationTime(char *path) {
	struct stat attr;
	stat(path, &attr);
	printf("Last modified time: %s", ctime(&attr.st_mtime));
	return ctime(&attr.st_mtime);
}
