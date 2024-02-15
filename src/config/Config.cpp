#include "../../include/config/Config.hpp"
// #include "Config.hpp"

Config::Config(): _rootPath("/Users/vlenard/Desktop/webserv/www")
{
	cout << "CONFIG CLASS CREATED" << endl;
	_defaultPath = _rootPath + "/index.html";
	_mimeTypes = parseMime();
}

map<string, string> Config::parseMime()
{
	if (DEBUG) cout << "PARSE MIME" << endl;
	map<string, string> map;
	string _whitespace = " \t\n\r\f\v";
	ifstream file;
	file.open("config/mime.types");
	if (!file)
		exitWithError("mime type file not found");
	if (DEBUG) cout << "bp1" << endl;
	string line;
	while (getline(file, line))
	{
		string s1; string s2;
		line.erase(0, line.find_first_not_of(_whitespace));
		int del = line.find_first_of(_whitespace);
		s1 = line.substr(0, del);
		if (line[del])
		{
			string buf = line.substr(del);
			s2 = buf.substr(buf.find_first_not_of(_whitespace));
		}
		createKeypairs(map, s1, s2);
	}
	if (DEBUG) cout << "bp4" << endl;
	file.close();
	return map;
}


void Config::createKeypairs(map<string, string> & map, string s1, string s2)
{
	if (!s1[0] || !s2[0])
		return ;
	string buf;
	string s = s2.substr(0, s2.find_first_of(";"));
	int i = 0;
	while (s[i])
	{
		int begin = i;
		while (s[i] && !isspace(s[i]))
			i++;
		if (i > begin)
		{
			buf = s.substr(begin, i - begin);
			map[buf] = s1;
		}
		if (isspace(s[i]))
			i++;
	}
}