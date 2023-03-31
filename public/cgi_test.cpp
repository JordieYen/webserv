#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

int	main(int argc, char** argv, char **envp)
{
	int	fds[2];

	pipe(fds);

	int pid = fork();

	if (pid == 0)
	{
		std::vector<char*>	args;
		args.push_back(strdup("/usr/bin/python3"));
		args.push_back(strdup("homepage.py"));
		args.push_back(nullptr);

		int		envp_len = 0;
		char**	temp = envp;

		while (*temp++)
			envp_len++;

		std::vector<char*>	new_envp(envp, envp + envp_len);
		new_envp.push_back(strdup("PATH_INFO=/"));
		new_envp.push_back(strdup("USERNAME=icons"));
		new_envp.push_back(nullptr);

		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
		close(fds[0]);
		execve("/usr/bin/python3", args.data(), new_envp.data());
	}
	else
	{
		close(fds[1]);

		waitpid(pid, NULL, 0);

		char*	buffer = static_cast<char*>(malloc(65535 * sizeof(char)));
		int	rv = read(fds[0], buffer, 65535);
		if (rv > 0)
		{
			std::ofstream	output("cgi_homepage.html");

			output << buffer;
			output.close();
		}
	}
}
