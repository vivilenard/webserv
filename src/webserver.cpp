#include "socketManager.hpp"
#include "Interface.hpp"

std::string testHttp(const std::string &request) {
    std::cout << "testHttp called" << std::endl;
    if (request.empty()) {
        std::cout << "request is empty" << std::endl;
        return "";
    } else {
        std::string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: text/html\r\n";
        httpResponse += "Content-Length: 210\r\n";
        httpResponse += "Connection: keep-alive\r\n";
        httpResponse += "\r\n";
        httpResponse += "<!DOCTYPE html>\r\n";
        httpResponse += "<html>\r\n";
        httpResponse += "<head>\r\n";
        httpResponse += "    <title>Hello World!</title>\r\n";
        httpResponse += "</head>\r\n";
        httpResponse += "<body style=\"background-color: #363636; color: #ffffff;\">\r\n";
        httpResponse += "    <p>Hey, you just connected through the best socket manager in the world.</p>\r\n";
        httpResponse += "</body>\r\n";
        httpResponse += "</html>\r\n";

        return httpResponse;
    }
}


int main()
{
	// int numWorker = 4;
	// parsing here
	// add application map to interface before forking the workers
	// applicationInterface::addApplication(80, /*&http::application*/);
	protocolFunction testFunction = &testHttp;
	Interface::addProtocol("HTTP/1.1", testFunction);

	// add sockets
	{
		socketManager::addSocket("0.0.0.0", 80, IPV4, TCP);
	}
	InterfaceFunction interfaceFunction = &Interface::interface;
	socketManager::start(interfaceFunction);
	return 0;
}
