#include "socketManager.hpp"
#include "Interface.hpp"
#include "../include/config/Config.hpp"
#include "../include/httpTransfer/Request.hpp"
#include "../include/httpTransfer/Response.hpp"
#include "../parse/configFile.hpp"


std::string testHttp(const std::string &request/* , Config & config */) {
    // std::cout << "testHttp called" << std::endl;
    if (request.empty()) {
        std::cout << "request is empty" << std::endl;
        return "";
    } else {
		cout << MAG << "-------request----->>" << NORM << endl;
		Request		httpRequest(request);
		Response	httpResponse(httpRequest);
		std::string response = httpResponse.getResponse();
		cout << BLUE << "RESPONSE:\n" << response << NORM <<endl;
		cout << MAG << "<<------------------------" << NORM << endl;
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
		socketManager::addSocket("0.0.0.0", 8888, IPV4, TCP);
	}
	InterfaceFunction interfaceFunction = &Interface::interface;
	socketManager::start(interfaceFunction);
	return 0;
}
