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

	int	SimpleServer::get_server_fd(void) const
	{
		return (this->_socket->get_fd());
	}

	ListeningSocket*	SimpleServer::get_socket(void) const
	{
		return (this->_socket);
	}

	void	SimpleServer::accepter(void)
	{
		struct sockaddr_in	address = this->_socket->get_address();
		int					addrlen = sizeof(address);

		this->_new_socket = accept(this->_socket->get_fd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
		this->_buffer = static_cast<char *>(malloc(30000 * sizeof(char)));
		read(this->_new_socket, this->_buffer, 30000);
		this->_content = string(this->_buffer);
		free(this->_buffer);
	}

	void	SimpleServer::handler(void)
	{
		std::cout << this->_content << std::endl;
	}
	
	void	SimpleServer::responder(void)
	{
		ifstream	index("index/index.html");
		string		content;

		index >> content;
		write(this->_new_socket, content.c_str(), content.length());
		// write(this->_new_socket, "Hello from server!", 19);
		close(this->_new_socket);
	}
}
