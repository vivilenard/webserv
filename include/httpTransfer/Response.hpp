#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Request.hpp"
// #include "../../parse/Config.hpp"
#include "Status.hpp"
#include "configFile.hpp"

#define STATUSCODE _statusCode.getStati()
struct configServer;

class Response
{
	static StatusCode	_statusCode;
	configServer		_configfile;
	Request				_request;
	string				_response;
	int					_status;
	int					_redir;
	string				_statusInfo;
	string				_httpVersion;
	string				_URI;
	string				_responseBody;
	string				_fileContent;
	string				_fileContentType;
	string				_cgiScript;
	COOKIES				_newCookies;
	COOKIES				_oldCookies;

	bool				invalidRequest();
	void				processGet();
	void				processPost();
	void				processDelete();
	bool 				methodAllowed();
	bool				readFile(const string & path);
	string				addRootPath(const string & path);
	string				redirectURI(string path);
	int					addDefaultFile(string & path);
	string				getContentType(const string & path);
	int					createFile(std::string & path, const std::string & cont);
	std::string			findKeyByValue(std::map<string, string>m, string value);
	void				formResponse(int status, const string & statusInfo);
	const string		generateDefaultHtml(const string & text);
	const string		createErrorBody(const int & status, const string & statusInfo);
	bool				isCgi(const string & path);
	//Multipart
	int					isMultipart();
	int					handleMultipart();
	bool				incorrectMimeType(const string & contentType);
	bool				noFilename();
	//DirectoryListing
	bool				isDirectory(const string & endpoint);
	bool				listDirectory(DIR* dirp);
	string				currentDir();
	//Cookies
	bool				recieveQuery(const string & contentType);
	void				handleQuery();
	void				bakeLoginCookie(Query & query);
	string				CookiesToHeaders();
	const string		getCookieBody();
	COOKIES				getSavedCookies(const string & filename);
	bool				insertNewToSavedCookies(COOKIES & fresh, COOKIES & saved);
	bool				saveCookiesInFile(const COOKIES & cookies, const string & filename);
	bool				IDcheck(const string & name);
	string				getFileCreationTime(char *path);

	public:

		Response(Request & request, configServer & configfile);
		const string		getResponse() const { return _response ; };
		bool				beginsWithDot(const string & s);
};

int executeCgi(std::string &cgiScript);
const string getDir(string & uri);
const string generateCookieDate();
const string	generateSequence();

# endif
