#include "webserv.hpp"
// #include <iostream>
// #include <vector>

int	main(int argc, char **argv)
{
	// (void)argc;
	// (void)argv;
	// AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10
	if (argc > 2)
	{
		std::cout << "Error: please provide up to a single configuration file..." << std::endl;
	}
	else if (argc == 2)
	{
		// ft::ConfigParser	parser(argv[1]);
		// parser.read_config();
		// print read config strings
		// std::vector<std::vector<std::vector<std::string> > > config_string = parser.get_server_configs();
		// for (std::vector<std::vector<std::vector<std::string> > >::iterator up_it = config_string.begin(); up_it != config_string.end(); up_it++)
		// {
		// 	std::cout << "ServerConfig: \n";
		// 	for (std::vector<std::vector<std::string> >::iterator it = (*up_it).begin(); it != (*up_it).end(); it++)
		// 	{
		// 		std::cout << "Token:";
		// 		for (std::vector<std::string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
		// 		{
		// 			std::cout << " [" << *it2 << "]";
		// 		}
		// 		std::cout << std::endl;
		// 	}
		// 	std::cout << std::endl;
		// }
		// parser.check_config();
		// parser.parse_config();
		// std::vector<ft::ServerConfig>	configs = parser.get_parsed_server_configs();
		// for (std::vector<ft::ServerConfig>::iterator it = configs.begin(); it != configs.end(); it++)
		// {
		// 	ft::ServerConfig::normalValueType	values = it->get_normal_directive("root");
		// 	// print location
		// 	// ft::ServerConfig::locationValueType location = it->get_location_directive("~");
		// 	// std::cout << location.get_match_type() << std::endl;
		// 	// ft::LocationBlock::valueType values = location.get_directive("root");
		// 	std::cout << "Tokens: ";
		// 	for (ft::ServerConfig::normalValueType::iterator it2 = values.begin(); it2 != values.end(); it2++)
		// 	{
		// 		std::cout << "[" << *it2 << "]";
		// 	}
		// 	std::cout << std::endl;
		// }
		ft::WebServer	webserv(argv[1]);
		webserv.launch();
	}
	else
	{
		ft::WebServer	webserv;
		webserv.launch();
	}
}

// #include <iostream>
// #include <vector>
// #include <map>

// int	main(void)
// {
// 	std::map<int, std::vector<int>* >	test;


// 	while (1)
// 	{
// 		test.insert(std::make_pair(80, new std::vector<int>(1, 42)));
// 		for (std::map<int, std::vector<int>* >::iterator it = test.begin(); it != test.end(); )
// 		{
// 			std::cout << it->first << std::endl;
// 			if (it->first == 80)
// 			{
// 				std::cout << it->first << std::endl;
// 				test.erase(it++);
// 			}
// 			else
// 				++it;
// 		}
// 	}
// }
