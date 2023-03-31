#include "SimpleServer.hpp"

namespace	ft
{
	SimpleServer::SimpleServer(ServerConfig config) : _config(config)
	{
		const char*	methods[] = {"GET", "POST"};
		
		this->_config.set_normal_directive("listen", ServerConfig::normalValueType(1, "80"));
		this->_config.set_normal_directive("root", ServerConfig::normalValueType(1, "public"));
		this->_config.set_normal_directive("index", ServerConfig::normalValueType(1, "index.html"));
		this->_config.set_normal_directive("allowed_methods", ServerConfig::normalValueType(methods, methods + 2));
		this->_config.set_normal_directive("client_max_body_size", ServerConfig::normalValueType(1, "10M"));

		int				port;
		istringstream	port_stream(this->_config.get_normal_directive("listen").front());
		
		port_stream >> port;
		this->_socket = new ListeningSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10);
		this->_cookies = new map<string, vector<string> >();
		this->_port = port;
	}

	SimpleServer::~SimpleServer()
	{
		delete this->_socket;
	}

	ServerConfig&	SimpleServer::get_config(void)
	{
		return (this->_config);
	}

	int	SimpleServer::get_port(void) const
	{
		return (this->_port);
	}

	ListeningSocket*	SimpleServer::get_socket(void) const
	{
		return (this->_socket);
	}

	int	SimpleServer::get_server_fd(void) const
	{
		return (this->_socket->get_fd());
	}

	int	SimpleServer::accept_connection(void)
	{
		struct sockaddr_in	address = this->_socket->get_address();
		int					addrlen = sizeof(address);
		int					fd;

		fd = accept(this->_socket->get_fd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
		return (fd);
	}

	map<string, vector<string> >	*SimpleServer::get_cookies(void) const
	{
		return (this->_cookies);
	}
}
