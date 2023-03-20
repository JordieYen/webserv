#include "webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "Error: please provide up to a single configuration file..." << std::endl;
	}
	else if (argc == 2)
	{
		ft::WebServer	webserv(argv[1]);
		webserv.launch();
	}
	else
	{
		ft::WebServer	webserv;
		webserv.launch();
	}
}
