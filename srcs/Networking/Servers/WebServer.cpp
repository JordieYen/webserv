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

	WebServer::~WebServer(void)
	{
		for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
		{
			delete server->second;
		}
	}

	void	WebServer::setup(void)
	{
		this->_parser.read_config();
		this->_parser.check_config();
		this->_parser.parse_config();

		std::vector<ft::ServerConfig>	configs = this->_parser.get_parsed_server_configs();

		for (std::vector<ft::ServerConfig>::iterator it = configs.begin(); it != configs.end(); it++)
		{
			SimpleServer	*new_server = new SimpleServer(*it);

			this->_servers.insert(std::make_pair(new_server->get_port(), new_server));
		}
	}

	void	WebServer::launch(void)
	{
		int	i = 0;
		while (true)
		{
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				server->second->accepter();
				server->second->handler();
				server->second->responder();
			}
			i++;
		}
	}
}
