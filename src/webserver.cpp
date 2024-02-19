#include "socketManager.hpp"
#include "Interface.hpp"
#include "../include/config/Config.hpp"
#include "../include/httpTransfer/Request.hpp"
#include "../include/httpTransfer/Response.hpp"


std::string testHttp(const std::string &request/* , Config & config */) {
    // std::cout << "testHttp called" << std::endl;
    if (request.empty()) {
        std::cout << "request is empty" << std::endl;
        return "";
    } else {
		cout << MAGCOLOR << "-------httpTransfer----->>" << NORM << endl;
		Request		httpRequest(request);
		cout << GREEN << request << NORM << endl;
		cout << GREEN << "------------" << NORM << endl;
		cout << RED << httpRequest << NORM << endl;
		Response	httpResponse(httpRequest);
		std::string response = httpResponse.getResponse();
		cout << BLUECOLOR << "RESPONSE:\n" << response << NORM <<endl;
		cout << MAGCOLOR << "<<------------------------" << NORM << endl;
		return response;
    }
}


int main()
{
	// int numWorker = 4;
	// parsing here
	// add application map to interface before forking the workers
	// applicationInterface::addApplication(80, /*&http::application*/);
	//Config config;
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
