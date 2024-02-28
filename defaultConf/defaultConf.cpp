#include "../parse/configFile.hpp"

void ConfigFile::runByDefault()
{
	configServer tmpServer;
	tmpServer._serverName = "default";
	tmpServer._listen = 80;
	tmpServer._address = "localhost";
	tmpServer.validFormat = true;
	tmpServer._locations["default"]._name = "default";
	tmpServer._locations["default"]._delete = false;
	tmpServer._locations["default"]._post = false;
	tmpServer._locations["default"]._get = false;
	tmpServer._locations["default"]._index = "false";
	this->_configMap["default"] = tmpServer;
}