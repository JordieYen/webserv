#include "SimpleServer.hpp"

namespace	ft
{
	SimpleServer::SimpleServer(ServerConfig config) : _config(config)
	{
		this->_config.set_normal_directive("listen", ServerConfig::normalValueType(1, "80"));
		this->_config.set_normal_directive("root", ServerConfig::normalValueType(1, "/index"));
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

	int	SimpleServer::get_pollfd_index(void) const
	{
		return (this->_pollfd_index);
	}

	void	SimpleServer::set_pollfd_index(int pollfd_index)
	{
		this->_pollfd_index = pollfd_index;
	}

	int	SimpleServer::accept_connection(void)
	{
		struct sockaddr_in	address = this->_socket->get_address();
		int					addrlen = sizeof(address);
		int					fd;

		std::cout << "Accepting fd: " << std::endl;
		fd = accept(this->_socket->get_fd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
		std::cout << "Accepted fd: " << fd << std::endl;
		return (fd);
	}

	// void	SimpleServer::handler(void)
	// {
	// 	std::cout << this->_content << std::endl;
	// }
	
	void	SimpleServer::respond(Request& request)
	{
		Response response(this->_config, request);

		response.handle_methods();
	}
}
