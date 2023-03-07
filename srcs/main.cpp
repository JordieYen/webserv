#include "webserv.hpp"

int	main(int argc, char **argv)
{
	// AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10
	if (argc > 2)
	{
		std::cout << "Error: please provide up to a single configuration file..." << std::endl;
	}
	else if (argc == 2)
	{
		std::string	path = argv[1];

		std::cout << "This is going to be parsed: " << path << std::endl;
	}
	else 
	{
		std::cout << "This will parse the default configuration file" << std::endl;
	}
}