#include "Response.hpp"

#define STATUSCODE _statusCode.getStati()

void Response::processDelete()
{
	if (PRINT) {cout << "DELETE REQUEST" << endl;}
	if (std::remove(_URI.c_str()) == 0)
	{
		_status = 204;
		_statusInfo = "Deleting successful";
		cout << RED << "Deleted: " << _URI << NORM << endl;
	}
	else
	{
		_status = 404;
		_statusInfo = "File Not Found";
		cout << RED << "DELETE: Did not find file: " << _URI << NORM << endl;
	}
	formResponse(_status, "");
}
