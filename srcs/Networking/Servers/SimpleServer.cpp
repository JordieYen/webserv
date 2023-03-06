#include "SimpleServer.hpp"

namespace	ft
{
	SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
	{
		this->_socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
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
}