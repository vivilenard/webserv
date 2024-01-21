#include "../../include/httpTransfer/httpTransfer.hpp"
// #include "httpTransfer.hpp"

Config httpTransfer::_config = Config();

httpTransfer::httpTransfer(const std::string & request):/* _socket_fd(fd) */_mimeType("html"), _request(request), _method("GET"),
	_prePath("www/"), _path(""), _contentLength(0)
{
	cout << "############REQUEST###############" << endl;
	cout << _request << endl;
	cout << "##################################" << endl;
	parseHeader();
	if (parseHeaderValue("Content-Length").length() > 0)
		_contentLength = atoi(parseHeaderValue("Content-Length").c_str());
	cout << "CONTLEN" << _contentLength << endl;
	if (DEBUG) cout << "METHOD:" << _method << "!" << endl;
}

string	httpTransfer::createResponse() // only GET
{
	cout << "EXCHANGE" << endl;
	std::string response;
	if (_method == "GET")
		response = getRequest();
	if (_method == "POST")
		response = postRequest();
	if (_method == "DELETE")
		response = delRequest();
	return response;
}

std::string httpTransfer::getRequest()
{
	if (DEBUG)
		cout << _prePath << "and " << _path << endl;
	_path = _prePath + _path;
	string filename = _prePath + "index.html";
	if (_path != _prePath)
		filename = _path;

	// if (DEBUG)
		cout << "PATHHHHJJHHHHH: " << filename << endl;
	ifstream file;
	file.open(filename.c_str());
	if (!file)
		exitWithError("file does not exist");
	ostringstream os;
	os << file.rdbuf();
	file.close();
	std::string filecontent = os.str();
	os.str("");
	string _mimeType = _path.substr(_path.find_first_of(".") + 1);
	if (DEBUG) cout << "MIME " << _mimeType << endl;

	os << "HTTP/1.1 ";
	os << "200 OK\n";
	os << "Content-Type:";
	os << _config._mimeTypes[_mimeType];
	os << "\nContent-Length: ";
	os << filecontent.length();
	os << "\n\n";
	os << filecontent;

	return os.str();
}

std::string httpTransfer::postRequest()
{
	cout << "POST REQUEST" << endl;
	std::string contentType = parseHeaderValue("Content-Type:");
	std::string body = parseBody();

	std::string fileType = findKeyByValue(_config._mimeTypes, contentType);
	cout << "TYPE:" << contentType << "," << fileType << "!" << endl;
	//cout << "BODY:" << endl << body << "!" << endl;

	//check if file already exists! if yes, overwrite and send different status code
	if (createFile(_path, body, fileType) == 0)
		cerr << "Post: Failed to create file" << endl;

	//if success: 201
	//If an existing resource is modified, either the 200 (OK) or 204 (No Content) response
	//The recipient of the entity MUST NOT ignore any Content-*(e.g. Content-Range) headers that it does 
	// not understand or implement and MUST return a 501 (Not Implemented) response in such cases.
	return _request;
}

int	httpTransfer::createFile(std::string & path, std::string & content, std::string & fileType)
{
	// int fd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY);
	// if (fd < 1 || access(path.c_str(), W_OK) < 0)
	// 	return false;
	// write (fd, path.c_str(), content.length());
	std::string filename = "post.";
	cout << "PATH:" << _path << "!" << endl;
	if (path == "")
	{
		cout << "filename: " << filename << ", filetype:" << fileType << endl;
		std::string buf = filename.append(fileType);
		buf = _prePath.append(filename);
		path = buf;
		cout << "FILEPATH = " << path << endl;
	}
	fstream file;
	if (access(path.c_str(), W_OK) > 0)
		cout << "file already exists and has writing rights, its going to be overwritten" << endl;
	file.open(path.c_str(), ios::trunc | ios::binary | ios::out);
	if (!file.is_open())
		return false;
	file << content;
	file.close();
	return true;
}

std::string	httpTransfer::findKeyByValue(std::map<string, string>m, string value)
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


std::string httpTransfer::delRequest()
{
	cout << "DELETE REQUEST" << endl;

	if (_path == "")
	{
		cout << "no file path found" << endl;
		return "";
	}
	FILE* fp = fopen(_path.c_str(), "r+");
	if (!fp)
	{
		cerr << "file path is wrong" << endl;
		return "";
	}
	std::remove(_path.c_str());
	fp = fopen(_path.c_str(), "r+");
	if (!fp)
	{
		cerr << "File successful deleted" << endl;
		return "";
	}
	return _request;
}

void httpTransfer::parseHeader()
{
	if (DEBUG) cout << "PARSE HEADER" << endl;
	istringstream is(_request);
	string line;
	vector <string> v;

	while (getline(is, line, ' '))
	{
		v.push_back(line);
		if (line.find("\n") != string::npos)
			break;
	}
	_method = v[0];
	if (v[1][0] && v[1] != "/")
		_path = v[1].substr(v[1].find_first_not_of("/"));
	_httpVersion = v[2].substr(0, v[2].find_first_of("\n"));
	if (DEBUG)
		cout << "HEADER: " << _method << ", " << _path << ", " << _httpVersion << endl;
}

std::string httpTransfer::parseBody()
{
	//UNTIL CONTENT LENGTH
	std::string body;
	int pos_body = findDoubleNewline(_request);
	if (pos_body < 0)
	{
		cerr << "No Body in Post request!" << endl;
		return "";
	}
	istringstream is(parseHeaderValue("Content-Length:"));
	int contentLength;
	is >> contentLength;
	// cout << "CONTLEN: " << contentLength << endl;
	// body = _request.substr(pos_body);
	body = _request.substr(pos_body, contentLength);
	return body;
}

int	httpTransfer::findDoubleNewline(std::string & s)
{
	int pos = s.find("\n\n");
	if (pos > 0)
		pos += 2;
	else
	{
		pos = s.find("\r\n\r\n");
		if (pos > 0)
			pos += 4;
		else
			return -1;
	}
	return pos;
}

std::string	httpTransfer::parseHeaderValue(std::string s)
{
	size_t pos_type = _request.find(s);
	//cout << "POS TYPE"<< pos_type << endl;
	if (pos_type == string::npos)
	{
		cerr << "No Content-Type found" << endl;
		return "";
	}
	std::string typeLine = _request.substr(pos_type, _request.find('\n', pos_type));
	typeLine = typeLine.substr(typeLine.find_first_of(':') + 1);
	typeLine = typeLine.substr(typeLine.find_first_not_of(' '));
	int pos = typeLine.find_first_of('\n');
	if ((int)typeLine.find_first_of('\r') < pos)
		pos = typeLine.find_first_of('\r');
	std::string contentType = typeLine.substr(0, pos);
	return contentType;
}
