#include "../../include/httpTransfer/Response.hpp"

#define STATUSCODE _statusCode.getStati()


bool	Response::isCgi(const string & path)
{
	size_t pos = path.find_last_of('.');
	if (pos == string::npos)
		return false;
	// cout << "SUB:" << path.substr(pos) << endl;
	if (path.substr(pos) == ".py")
	{
		cout << "its a python cgi" << endl;
		return true;
	}
	return false;
}