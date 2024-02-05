// #include "httpTransfer.hpp"

// httpTransfer::httpTransfer(int fd, Config configuration):_socket_fd(fd), _mimeType("html"), _method("GET"), _path("/"), _config(configuration)
// {
// 	_request = readRequest();

// 	parseHeader();
// 	cout << _request << endl;
// }

// string httpTransfer::readRequest()
// {
// 	if (DEBUG) cout << "READ REQUEST" << endl;
// 	const int BUFFER_SIZE = 30720;

// 	char buffer[BUFFER_SIZE] = {0};
// 	int bytesReceived = read(_socket_fd, buffer, BUFFER_SIZE);
// 	if (bytesReceived < 0)
// 		exitWithError("Failed to read bytes from client socket connection");
// 	string bufferr = buffer;
// 	return (bufferr);
// }


// void httpTransfer::parseHeader()
// {
// 	if (DEBUG) cout << "PARSE HEADER" << endl;
// 	istringstream is(_request);
// 	string line;
// 	vector <string> v;

// 	while (getline(is, line, ' '))
// 	{
// 		v.push_back(line);
// 		if (line.find("\n") != string::npos)
// 			break;
// 	}
// 	_method = v[0];
// 	if (v[1][0] && v[1] != "/")
// 		_path = v[1].substr(v[1].find_first_not_of("/"));
// 	_httpVersion = v[2].substr(0, v[2].find_first_of("\n"));
// 	if (DEBUG) cout << "HEADER: " << _method << ", " << _path << ", " << _httpVersion << endl;
// }


// void	httpTransfer::exchange() // only GET
// {
// 	if (DEBUG) cout << "EXCHANGE" << endl;
// 	//char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
// 	string filename = "index.html";
// 	if (_path != "/")
// 		filename = _path;
// 	if (filename == "favicon.ico")
// 		return ;
// 	ifstream file;
// 	file.open(filename.c_str());
// 	if (!file)
// 		exitWithError("file does not exist");
// 	ostringstream os;
// 	os << file.rdbuf();
// 	file.close();
// 	std::string filecontent = os.str();
// 	os.str("");
// 	string _mimeType = _path.substr(_path.find_first_of(".") + 1);
// 	if (DEBUG) cout << "MIME " << _mimeType << endl;

// 	os << "HTTP/1.1 ";
// 	os << "200 OK\n";
// 	os << "Content-Type:";
// 	os << _config._mimeTypes[_mimeType];
// 	os << "\nContent-Length: ";
// 	os << filecontent.length();
// 	os << "\n\n";
// 	os << filecontent;

// 	std::string http_string = os.str();
// 	const char *http = http_string.c_str();
// 	int bytes_sent = send(_socket_fd, http, http_string.length(), 0);
// 	if (bytes_sent < 0)
// 		exitWithError("Problem with sending via socket");
// }

