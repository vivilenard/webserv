#include "Error.hpp"

void exitWithError(const std::string &errorMessage)
{
	cout << "ERROR: " << errorMessage << endl;
	for (int i = getdtablesize(); i > 3;) close(--i);
	exit(1);
}