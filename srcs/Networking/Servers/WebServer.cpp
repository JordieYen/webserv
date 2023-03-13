#include "WebServer.hpp"

namespace ft
{
	WebServer::WebServer(void) : _parser(DEFAULT_CONFIG_PATH)
	{
		this->setup();
	}

	WebServer::WebServer(std::string config_path) : _parser(config_path)
	{
		this->setup();
	}

	WebServer::~WebServer(void) {}

	void	WebServer::setup(void)
	{
		this->_parser.read_config();
		this->_parser.check_config();
		this->_parser.parse_config();

		std::vector<ft::ServerConfig>	configs = this->_parser.get_parsed_server_configs();

		for (std::vector<ft::ServerConfig>::iterator it = configs.begin(); it != configs.end(); it++)
		{
			std::cout << "START CREATE: " << std::endl;
			SimpleServer	new_server(*it);
			std::pair<int, SimpleServer>	unfunnypair(new_server.get_port(), new_server);
			std::cout << "POST UNFUNNYPAIR CREATION: " << std::endl;
			std::cout << "SOCKET_ADDR BEFORE INSERT: " << unfunnypair.second.get_socket() << std::endl;
			std::cout << "SOCKET_FD BEFORE INSERT: " << unfunnypair.second.get_server_fd() << std::endl;
			this->_servers.insert(unfunnypair);
			std::cout << "SOCKET_ADDR AFTER INSERT: " << this->_servers.at(81).get_socket() << std::endl;
			std::cout << "SOCKET_FD AFTER INSERT: " << this->_servers.at(81).get_server_fd() << std::endl;
			std::cout << "END CREATE: " << std::endl;
		}
	}

	void	WebServer::launch(void)
	{
		while (true)
		{
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				server->second.accepter();
				// server->second.handler();
				server->second.responder();
			}
		}
	}
}
