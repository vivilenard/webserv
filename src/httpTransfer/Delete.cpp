#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processDelete()
{
	cout << "DELETE REQUEST" << endl;
	if (std::remove(_URI.c_str()) == 0)
		_status = 200;
	else 
		_status = 404;
	formResponse(_status, "");
}
