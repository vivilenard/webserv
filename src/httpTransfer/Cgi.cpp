#include "../../include/httpTransfer/Response.hpp"
#include "../../include/httpTransfer/Request.hpp"


#define STATUSCODE _statusCode.getStati()

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>

/*print("SERVER_SOFTWARE:", os.environ.get("SERVER_SOFTWARE"))
print("SERVER_NAME:", os.environ.get("SERVER_NAME"))
print("SERVER_PORT:", os.environ.get("SERVER_PORT"))
print("REQUEST_METHOD:", os.environ.get("REQUEST_METHOD"))
print("PATH_INFO:", os.environ.get("PATH_INFO"))
print("QUERY_STRING:", os.environ.get("QUERY_STRING"))
print("CONTENT_TYPE:", os.environ.get("CONTENT_TYPE"))
print("CONTENT_LENGTH:", os.environ.get("CONTENT_LENGTH"))
print("HTTP_USER_AGENT:", os.environ.get("HTTP_USER_AGENT"))
print("HTTP_REFERER:", os.environ.get("HTTP_REFERER"))
print("HTTP_HOST:", os.environ.get("HTTP_HOST"))*/


void Request::buildCgiEnv()
{
	this->_envCgi["method"] = "REQUEST_METHOD=" + this->_method;
	std::cout << "Method ---> " << this->_envCgi["method"] << "$"<< std::endl;
	Headers::iterator it;
	for (it  = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first == "Content-Length")
		{
			this->_envCgi["content-length"] = "CONTENT_LENGTH=" + it->second;
			std::cout << "CONTENT_LENGTH " << this->_envCgi["content-length"] << "$" << std::endl;
			break;
		}
	}
	std::cout << "THIS IS YOUR QUERY ---> "<< this->_envCgi["query"] << std::endl;
	//querry added in parseQuery
}

int Request::executeCgi(std::string &cgiScript) {
	// Create pipes for stdin and stdout
	int pipe_stdin[2];
	int pipe_stdout[2];
	pipe(pipe_stdin);
	pipe(pipe_stdout);
	// Fork a child process
	pid_t pid = fork();
	if (pid == -1) {
		std::cerr << "Error forking process\n";
		return EXIT_FAILURE;
	} else if (pid == 0) { // Child process
		// Redirect stdin and stdout to pipes
		dup2(pipe_stdin[0], STDIN_FILENO);
		dup2(pipe_stdout[1], STDOUT_FILENO);

		// Close unused pipe ends
		close(pipe_stdin[1]);
		close(pipe_stdout[0]);
		// Execute the CGI script
		const char *command[] = {"/usr/bin/python", "/Users/apaghera/Documents/webServ/cgi-bin/test.py", NULL};
		std::vector<const char *>env;
		for (EnvCgi::iterator it = this->_envCgi.begin(); it != this->_envCgi.end(); ++it)
				env.push_back(const_cast<char *>(it->second.c_str()));
		env.push_back(NULL);
		execve("/usr/bin/python", (char* const*)command, const_cast<char *const *>(env.data()));
		// If execve returns, an error occurred
		std::cerr << "Error executing CGI script\n";
		return EXIT_FAILURE;
	} else { // Parent process
		// Close unused pipe ends
		close(pipe_stdin[0]);
		close(pipe_stdout[1]);

		// Read data from the CGI script and send it to the client
		char buffer[1024];
		ssize_t bytes_read;
		while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer))) > 0) {
			// Append the read data to the output string
			cgiScript.append(buffer, bytes_read);
		}
		if (bytes_read == -1) {
			// Error reading from pipe
			std::cerr << "Error reading from pipe\n";
			close(pipe_stdout[0]);
			return EXIT_FAILURE;
		}

		// Close the read end of the pipe
		close(pipe_stdout[0]);

		// Wait for the child process to finish
		waitpid(pid, NULL, 0);
	}

	return EXIT_SUCCESS;
}


bool	Response::isCgi(const string & path)
{
	size_t pos = path.find_last_of('.');
	if (pos == string::npos)
		return false;
	if (path.substr(pos) == ".py")
	}
		_request.buildCgiEnv();
		_request.executeCgi(this->_cgiScript);
		_fileContentType = "text/html";
		int pos_nl = _request.findDoubleNewline(_cgiScript);
		_cgiScript = _cgiScript.substr(pos_nl);
		_status = 200;
		return true;
	}
	return false;
}