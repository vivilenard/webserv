#include "httpTransfer.hpp"

void	httpTransfer::exchange(int fd)
{
	const int BUFFER_SIZE = 30720;

	char buffer[BUFFER_SIZE] = {0};
	int bytesReceived = read(fd, buffer, BUFFER_SIZE);
	if (bytesReceived < 0)
		exitWithError("Failed to read bytes from client socket connection");
	
	cout << "RECIEVED: \n" << buffer << endl;

	char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
	std::cout << "RESPONSE: \n" << arr << endl; 
	write(fd, arr, sizeof(arr));
}