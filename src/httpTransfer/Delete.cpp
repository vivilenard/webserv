#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processDelete()
{
	cout << "DELETE REQUEST" << endl;
	cout << RED << "Deleting: " << _URI << NORM << endl;
	if (std::remove(_URI.c_str()) == 0)
	{
		_status = 204;
		_statusInfo = "Deleting successful";
	}
	else
	{
		_status = 404;
		_statusInfo = "File Not Found";
	}
	formResponse(_status, "");
}
