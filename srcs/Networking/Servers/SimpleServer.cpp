#include "SimpleServer.hpp"

namespace	ft
{
	SimpleServer::SimpleServer(ServerConfig config) : _config(config)
	{
		this->_config.set_normal_directive("listen", ServerConfig::normalValueType(1, "80"));
		this->_config.set_normal_directive("root", ServerConfig::normalValueType(1, "public"));
		this->_config.set_normal_directive("index", ServerConfig::normalValueType(1, "index.html"));

		int				port;
		istringstream	port_stream(this->_config.get_normal_directive("listen").front());
		
		port_stream >> port;
		this->_socket = new ListeningSocket(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10);
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
}
