#include "Status.hpp"

StatusCode::StatusCode()
{
    setStatusMap();
}

void StatusCode::setStatusMap()
{
    _statusMap[7]   = "This is a directory";
    _statusMap[100] = "100 Continue";
    _statusMap[200] = "200 OK";
    _statusMap[201] = "201 Created";
    _statusMap[204] = "204 No Content";
	_statusMap[301] = "301 Moved Permanently";
	_statusMap[302] = "302 Found";
	_statusMap[308] = "308 Permanent Redirect";
    _statusMap[400] = "400 Bad Request";
    _statusMap[403] = "403 Forbidden";
    _statusMap[404] = "404 Not Found";
    _statusMap[405] = "405 Method Not Allowed";
    _statusMap[411] = "411 Length Required";
    _statusMap[415] = "415 Unsupported Media Type";
    _statusMap[418] = "418 I'm a teapot";
    _statusMap[500] = "500 Internal Server Error";
    _statusMap[501] = "501 Not Implemented";
    _statusMap[502] = "502 Bad Gateway";
    _statusMap[503] = "503 Service Unavailable";
    _statusMap[504] = "504 Gateway Timeout";
    _statusMap[505] = "505 HTTP Version Not Supported";

}
