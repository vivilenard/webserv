#include "../../include/httpTransfer/httpTransfer.hpp"
// #include "httpTransfer.hpp"

Config httpTransfer::_config = Config();

httpTransfer::httpTransfer(const std::string & request):/* _socket_fd(fd) */_mimeType("html"), _request(request), _method("GET"), _prePath("www/"), _path("")
{
	parseHeader();
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
		cout << _prePath << "and " << _path << endl;
	_path = _prePath + _path;
	if (DEBUG)
		cout << "HEADER: " << _method << ", " << _path << ", " << _httpVersion << endl;
}


string	httpTransfer::exchange() // only GET
{
	if (DEBUG) cout << "EXCHANGE" << endl;

	string filename = _prePath + "index.html";
	if (_path != _prePath)
		filename = _path;

	if (DEBUG)
		cout << "PATH: " << filename << endl;
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
