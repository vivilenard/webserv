#include "Response.hpp"

const string	generateSequence()
{
    srand((unsigned int)time(nullptr)); 
	int random = rand();
	ostringstream os;
	os << random;
	return os.str();
}
void printMAP(stringMAP m)
{
	stringMAP::iterator it = m.begin();
	for (; it != m.end(); it++)
		cout << it->first << " , " << it->second << endl;
	return ;
}
void	Response::bakeLoginCookie(Query & query)
{
	cout << "in bake cookie" << endl;
	string loginField = "42login";
	string session_id;
	// cout << ORANGE << "I will create a cookie. YUMM" << NORM << endl;
	_oldCookies = getSavedCookies("Cookies.csv");  //read from file
	COOKIES savedCookies = _oldCookies;
	if (PRINT) { cout << RED << "saved Cookies" << NORM << endl; printMAP(savedCookies); }
	if (savedCookies.empty())
		if (PRINT) {cout << "No Cookies are stored" << endl;}
	if (query.find(loginField)!= query.end())
	{
		string login_name = query[loginField];
		if (PRINT) {cout << "login: " << login_name << endl;}
		if (!login_name.empty())
		{
			session_id.append(generateSequence());  //generate id number
			_newCookies[login_name] = session_id;
			if (savedCookies.find(login_name) != savedCookies.end())
				cout << RED << "YOU ALREADY LOGGED IN" << NORM << endl;
		}
	}
	insertNewToSavedCookies(_newCookies, savedCookies);
	if (PRINT) { cout << RED << "saved Cookies after login" << NORM << endl; printMAP(savedCookies); }
	saveCookiesInFile(savedCookies, "Cookies.csv");
}

COOKIES		Response::getSavedCookies(const string & filename)
{
	COOKIES savedCookies;
	string name; string id; string time;
	ifstream cookieFile(filename);
	if (!cookieFile.is_open())
		return savedCookies;
	string buffer;
	// cout << ORANGE << "saved cookies string: " << NORM << endl;
	while(getline(cookieFile, buffer, '\n'))
	{
		//save it in savedCookies
		// cout << buffer << endl;
		istringstream is(buffer);
		getline(is, name, ';');
		getline(is, id, ';');
		getline(is, time, ';');
		savedCookies[name] = id;
	}
	return savedCookies;
}

bool		Response::insertNewToSavedCookies(COOKIES & fresh, COOKIES & saved)
{
	//inserts only if it doesnt exist yet
	COOKIES::iterator it = fresh.begin();
	for (; it != fresh.end(); it++)
	{
		if (saved.find(it->first) == saved.end())  //not found in saved
			saved[it->first] = it->second;
	}
	return true;
}

bool	Response::saveCookiesInFile(const COOKIES & cookies, const string & filename)
{
	std::ofstream ofile;
	ofile.open(filename);
	
	COOKIES::const_iterator it = cookies.begin();
	for (; it != cookies.end(); it++)
		ofile << it->first << ";" << it->second << ";" << "\n"; //generateCookieDate() << ";" << "\n";
	ofile.close();
	return true;
}

const string generateCookieDate()
{
	char dateStr[100];
	//Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	//Expires=Tue, 15 Feb 2020 00:17:48 GMT;
	time_t now = time(0); // get current dat/time with respect to system
	tm* gm_tm = gmtime(&now);
	strftime(dateStr, 100, "%a, %d %b %Y %T GMT", gm_tm);
	return dateStr;
}

string	Response::CookiesToHeaders()
{
	ostringstream os;
	COOKIES::iterator it = _newCookies.begin();
	for (; it != _newCookies.end(); it++)
	{
		if (_request.getCookies().find(it->first) == _request.getCookies().end())
		{
			os << "Set-Cookie: " << it->first << "=" << it->second
				// << "; Expires=" << generateExpiration() << "\r\n";
				<< "; Max-Age=" << 600 << ";" << "\r\n";
		}
	}
	return os.str();
}

const string Response::getCookieBody()
{
	COOKIES::iterator it = _newCookies.begin();
	for (; it != _newCookies.end(); it++)
		cout << GREEN << it->first << " = " << it->second << NORM << endl;

	string intraName = _newCookies.begin()->first;
	if (_request.getCookies().find(intraName) != _request.getCookies().end())
	{
		//user is already in client cookies
		if (IDcheck(intraName) == true)
		{
			cout << RED << "user is already logged in." << endl;
			return "Hello again! Seems like you logged in before ..";
		}
		else
		{
			cout << RED << "Cookie ID check failed" << NORM << endl;
			return "Your Cookie IDs are not matching";
		}
	}
	return ("Welcome " + intraName);
}

bool Response::IDcheck(const string & name)
{
	COOKIES clientCookies = _request.getCookies();
	COOKIES serverSession = _oldCookies;

	if (clientCookies.find(name) == clientCookies.end()
		|| serverSession.find(name) == serverSession.end())
	{
		//token not present in client and server
		return false;
	}
	string clientID = clientCookies[name];
	string sessionID = serverSession[name];
	cout << "cookie name: " << name << endl;
	cout << "client id: " << clientID << endl;
	cout << "session id: " << sessionID << endl;
	if (clientID == sessionID)
		return true;
	cout << "cookie IDs are not matching!" << endl;
	return false;
}