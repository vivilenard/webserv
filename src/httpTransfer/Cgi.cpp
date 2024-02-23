#include "../../include/httpTransfer/Response.hpp"

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

int executeCgi(std::string &cgiScript) {
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
        char *envp[] = {NULL}; // You may need to set up additional environment variables
        execve("/usr/bin/python", (char* const*)command, envp);

        // If execve returns, an error occurred
        std::cerr << "Error executing CGI script\n";
        return EXIT_FAILURE;
    } else { // Parent process
        // Close unused pipe ends
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        // Send data to the CGI script (if needed)
        // Write to pipe_stdin[1]

        // Read data from the CGI script and send it to the client
        char buffer[1024];
        ssize_t bytes_read;
		while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer))) > 0) {
            // Send the output to the client (e.g., over a network connection)
			cgiScript += buffer;
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
	// cout << "SUB:" << path.substr(pos) << endl;
	if (path.substr(pos) == ".py")
	{
		cout << "its a python cgi" << endl;
		cout << "------------------------------------------" << endl;
		executeCgi(this->_cgiScript);
		cout << "------------------------------------------" << endl;

		// _fileContentType = text/html
		//_cgiScript == html 
		_fileContentType = "text/html";
		int pos_nl = _request.findDoubleNewline(_cgiScript);
		cout << "NL: " << pos_nl << endl;
		_cgiScript = _cgiScript.substr(pos_nl);

		std::cout << "CGI:" << endl;
		std::cout << this->_cgiScript;
		_status = 200;
		return true;
	}
	return false;
}