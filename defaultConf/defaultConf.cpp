#include "../parse/configFile.hpp"

void ConfigFile::runByDefault()
{
	configServer tmpServer;
	tmpServer._serverName = "default";
	tmpServer._listen = 8080;
	tmpServer._root = "/Users/apaghera/Documents/webServ";
	tmpServer._address = "0.0.0.0";
	tmpServer._index = "home.html";
	tmpServer._socketAddress.interfaceAddress = convertToSockAddr(tmpServer._address.c_str(), tmpServer._listen);
	tmpServer._socketAddress.protocol = TCP;
	tmpServer.validFormat = true;
	tmpServer._locations["default"]._name = "default";
	tmpServer._locations["default"]._delete = false;
	tmpServer._locations["default"]._post = true;
	tmpServer._locations["default"]._get = true;
	tmpServer._locations["default"]._index = "false";
	this->_configMap["default"] = tmpServer;
}
