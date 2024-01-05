#include "../../include/error/Error.hpp"
// #include "Error.hpp"
void exitWithError(const std::string &errorMessage)
{
	std::cout << "ERROR: " << errorMessage << std::endl;
	for (int i = getdtablesize(); i > 3;) close(--i);
	exit(1);
}