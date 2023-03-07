#include "SimpleServer.hpp"

namespace	ft
{
	SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
	{
		this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
		launch();
	}

	SimpleServer::~SimpleServer()
	{
		std::cout << "Destructor called\n";
		delete this->_socket;
	}

	ListeningSocket	*SimpleServer::get_socket(void)
	{
		return (this->_socket);
	}

	void	SimpleServer::launch(void)
	{
		while (true)
		{
			std::cout << "Waiting...\n";
			this->accepter();
			this->handler();
			this->responder();
			std::cout << "Done!!!\n";
		}
	}

	void	SimpleServer::accepter(void)
	{
		struct sockaddr_in	address = this->get_socket()->get_address();
		int					addrlen = sizeof(address);

		this->_new_socket = accept(this->get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
		this->_buffer = static_cast<char *>(malloc(30000 * sizeof(char)));
		read(this->_new_socket, this->_buffer, 30000);
		this->_content = std::string(this->_buffer);
		free(this->_buffer);
	}

	void	SimpleServer::handler(void)
	{
		std::cout << this->_content << std::endl;
	}
	
	void	SimpleServer::responder(void)
	{
		write(this->_new_socket, "Hello from server!", 19);
		close(this->_new_socket);
	}
}
