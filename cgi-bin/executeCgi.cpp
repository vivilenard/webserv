#include <cstdlib>
#include <iostream>
#include <unistd.h>

int	executeCgi(void)
{
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
        char *argv[] = {"/usr/bin/python", "cgi-bin/test.py", NULL};
        char *envp[] = {NULL}; // You may need to set up additional environment variables
        if (!execve("/usr/bin/python", argv, envp))
			std::cout << "Nope! " << std::endl;

        // If execve returns, an error occurred
        std::cerr << "Error executing CGI script\n";
        return EXIT_FAILURE;
    } else { // Parent process
        // Close unused pipe ends
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        // Send data to the CGI script
        // Write to pipe_stdin[1]

        // Read data from the CGI script
        // Read from pipe_stdout[0]

        // Wait for the child process to finish
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;
}
