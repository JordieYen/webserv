#include "ListeningSocket.hpp"

namespace ft
{
	ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
	{
		this->_backlog = backlog;
		this->test_connection(this->start_listening());
	}

	ListeningSocket::~ListeningSocket() {}

	int	ListeningSocket::start_listening(void)
	{
		return (listen(this->get_fd(), this->_backlog));
	}

}