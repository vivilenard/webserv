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

int	sendStatus(std::string msg)
{
	std::cerr << msg << std::endl;
	return (EXIT_FAILURE);
}

int checkTime(timeval startTime, timeval currentTime, int timeOut)
{
	if (currentTime.tv_sec - startTime.tv_sec > timeOut)
		return (true);
	else
		return (false);
}

int handleExitStatus(int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		sendStatus( "error in python script");
		std::cerr << "STATUS " <<  WEXITSTATUS(status) << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int Request::executeCgi(std::string &cgiScript, const string & path)
{
	int pipe_stdin[2];
	int pipe_stdout[2];

	pipe(pipe_stdin);
	pipe(pipe_stdout);
	pid_t pid = fork();
	if (pid == -1)
		return (sendStatus("Error forking process"));
	else if (pid == 0)
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
			return (sendStatus("Error executing CGI script"));
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
			overTime = checkTime(startTime, currentTime, timeOut);
			if (overTime)
				break;
		}
		if (bytes_read == -1)
			return_value = sendStatus("Error reading from pipe");
		if (overTime)
		{
			kill(pid, SIGKILL);
			close(pipe_stdout[0]);
			return_value = sendStatus("TIMEOUT!!");
		}
		close(pipe_stdout[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			return_value = handleExitStatus(status);
		else
			return_value = sendStatus("Python script exit abnormally");
		return (return_value);
	}
	return (EXIT_SUCCESS);
}

bool	Response::isCgi(const string & path)
{
	size_t pos = path.find_last_of('.');
	int returnValue = EXIT_SUCCESS;
	bool absolute = true;
	if (pos == string::npos)
		return false;
	if (path.substr(pos) == ".py")
	{
		std::string tmp = currentPath();
		if (_request.getURI().find("/cgi-bin") != std::string::npos)
			absolute = false;
		std::string dir = tmp + (absolute ? "/cgi-bin" : "") + _request.getURI();
		if (!checkRoot(dir))
		{
			std::cerr << "Wrong CGI path!" << std::endl;
			_status = 404;
			return (false);
		}
		_request.buildCgiEnv();
		returnValue = _request.executeCgi(this->_cgiScript, dir);
		_fileContentType = "text/html";
		if (!_cgiScript.empty())
		{
			int pos_nl = _request.findDoubleNewline(_cgiScript);
			if (pos_nl > 0)
				_cgiScript = _cgiScript.substr(pos_nl);
		}
		if (returnValue == EXIT_FAILURE)
			_status = 404;
		else
			_status = 200;
		return true;
	}
	return false;
}
