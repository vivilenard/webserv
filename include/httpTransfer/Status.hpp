#ifndef STATUS_HPP
#define STATUS_HPP

#include <map>
#include <iostream>

#define statusMAP map<int,string>
using namespace std;

class StatusCode
{
	statusMAP		_statusMap;
	void			setStatusMap();

	public:
		StatusCode();
		statusMAP & getStati(){ return _statusMap; };

};

#endif
