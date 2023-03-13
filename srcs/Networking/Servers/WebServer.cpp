#include "WebServer.hpp"

namespace ft
{
	WebServer::WebServer(void)
	{

	}

	WebServer::WebServer(std::string input_file) : parser(input_file)
	{
		this->parser.read_config();
		this->parser.check_config();
		this->parser.parse_config();

		std::vector<ft::ServerConfig>	configs = parser.get_parsed_server_configs();

		for (std::vector<ft::ServerConfig>::iterator it = configs.begin(); it != configs.end(); it++)
		{
			ft::ServerConfig::normalValueType	values = it->get_normal_directive("root");
			// print location
			// ft::ServerConfig::locationValueType location = it->get_location_directive("~");
			// std::cout << location.get_match_type() << std::endl;
			// ft::LocationBlock::valueType values = location.get_directive("root");
			std::cout << "Tokens: ";
			for (ft::ServerConfig::normalValueType::iterator it2 = values.begin(); it2 != values.end(); it2++)
			{
				std::cout << "[" << *it2 << "]";
			}
			std::cout << std::endl;
		}
	}

	WebServer::~WebServer(void)
	{

	}

	void	WebServer::launch(void)
	{
		
	}
}