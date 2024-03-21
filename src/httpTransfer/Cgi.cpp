#include "Response.hpp"
#include "Request.hpp"
#include "configFile.hpp"
#include <sys/time.h>
#include <signal.h>

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
	if (!this->getBody().empty())
		this->_envCgi["post_data"] = "POST_DATA=" + this->getBody();
	Headers::iterator it;
	for (it  = _headers.begin(); it != _headers.end(); it++)
	{
		if (it->first == "Content-Length")
		{
			this->_envCgi["content-length"] = "CONTENT_LENGTH=" + it->second;
			break ;
		}
	}
}

std::string currentPath()
{
	char cwd[PATH_MAX];

	std::string path = std::string(getcwd(cwd, sizeof(cwd)));
	if (path.empty())
	{
		std::cerr << "Wrong path for cgi!" << std::endl;
		return ("");
	}
	return (path);
}

void closePipes(int *pipeIn, int *pipeOut)
{
	close(pipeIn[0]);
	close(pipeIn[1]);
	close(pipeOut[0]);
	close(pipeOut[1]);
}

int Request::executeCgi(std::string &cgiScript, const string & path)
{
	int pipe_stdin[2];
	int pipe_stdout[2];

	pipe(pipe_stdin);
	pipe(pipe_stdout);
	pid_t pid = fork();
	if (pid == -1)
	{
		std::cerr << "Error forking process\n";
		return EXIT_FAILURE;
	} else if (pid == 0)
	{
		dup2(pipe_stdin[0], STDIN_FILENO);
		dup2(pipe_stdout[1], STDOUT_FILENO);
		closePipes(pipe_stdin, pipe_stdout);
		const char *command[] = {"/usr/bin/python", path.c_str(), NULL};
		std::vector<const char *>env;
		for (EnvCgi::iterator it = this->_envCgi.begin(); it != this->_envCgi.end(); ++it)
				env.push_back(const_cast<char *>(it->second.c_str()));
		env.push_back(NULL);
		if (execve(command[0], (char* const*)command, const_cast<char *const *>(env.data())) < 0)
		{
			std::cerr << "Error executing CGI script\n";
			return EXIT_FAILURE;
		}
	}
	else
	{
		int status;
		int return_value = EXIT_SUCCESS;
		int timeOut = 5;
		bool overTime = false;
		close(pipe_stdin[0]);
		close(pipe_stdin[1]);
		close(pipe_stdout[1]);
		char buffer[1024];
		struct timeval startTime, currentTime;
		gettimeofday(&startTime, NULL);
		ssize_t bytes_read;
		while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer))) > 0)
		{
			cgiScript.append(buffer, bytes_read);
			gettimeofday(&currentTime, NULL);
			if (currentTime.tv_sec - startTime.tv_sec > timeOut)
			{
				overTime = true;
				break ;
			}
		}
		if (bytes_read == -1)
		{
			std::cerr << "Error reading from pipe\n";
			return_value = EXIT_FAILURE;
		}
		else if (bytes_read == 0)
			std::cout << "End of file reached (child process closed its stdout)\n"; // Debugging output
		if (overTime)
		{
			kill(pid, SIGKILL);
			close(pipe_stdout[0]);
			return EXIT_FAILURE;
		}
		close(pipe_stdout[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			if(WEXITSTATUS(status) != 0)
				std::cerr << "error in python script " <<  WEXITSTATUS(status) << std::endl;
			return_value = EXIT_FAILURE;
		}
		else
		{
			std::cerr << "Python script exit abnormally" << std::endl;
			return_value = EXIT_FAILURE;
		}
		return (return_value);
	}
	return (EXIT_SUCCESS);
}

bool	Response::isCgi(const string & path)
{
	size_t pos = path.find_last_of('.');
	if (pos == string::npos)
		return false;
	if (path.substr(pos) == ".py")
	{
		std::string tmp = currentPath();
		std::string dir = path.substr(1) + "/cgi-bin"; // <----- if the root = "./"
		/*std::cout << "CGI PATH---> " << path.c_str() << std::endl; // <----- if you have an absolute  */
		std::cout << "CURRENT PATH" << tmp << std::endl;
		std::cout << "CGI  DIR PATH---> " << dir << std::endl;
		if (!checkRoot(dir))
		{
			std::cerr << "Wrong CGI path!" << std::endl;
			return (false);
		}
		_request.buildCgiEnv();
		_request.executeCgi(this->_cgiScript, dir);
		_fileContentType = "text/html";
		if (!_cgiScript.empty())
		{
			int pos_nl = _request.findDoubleNewline(_cgiScript);
			if (pos_nl > 0)
				_cgiScript = _cgiScript.substr(pos_nl);
		}
		_status = 200;
		return true;
	}
	return false;
}
