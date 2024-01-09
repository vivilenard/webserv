#include "httpTransfer.hpp"

void	httpTransfer::exchange(int fd)
{
	const int BUFFER_SIZE = 30720;

	char buffer[BUFFER_SIZE] = {0};
	int bytesReceived = read(fd, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		exitWithError("Failed to read bytes from client socket connection");
	
	cout << "RECIEVED: \n" << buffer << endl;

	//char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
	std::string filename = "bild.jpg";
	ifstream file;
	file.open(filename);
	if (!file)
		exitWithError("file does not exist");
	ostringstream os;
	os << file.rdbuf();
	std::string filecontent = os.str();

	//cout << "THIS IS FILE" << endl << filecontent.str() << filecontent.str().length() << endl;
	ostringstream ss;
	ss << "HTTP/1.1 ";
	ss << "200 OK\n";
	ss << "Content-Type:";
	ss << "image/jpeg\n";
	ss << "Content-Length: ";
	ss << filecontent.length();
	ss << "\n\n";
	ss << filecontent;

	//cout << filecontent << endl;
	std::string http_string = ss.str();
	std::cout << "RESPONSE: \n" << http_string << endl;
	const char *http = http_string.c_str();
	printf("STRING\n %s\n", http);
	write(fd, http , strlen(http));
	//implement send and recieve !!
}
