#include "ListeningSocket.hpp"

namespace ft
{
	ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
	{
		this->_backlog = backlog;
		this->start_listening();
		test_connection(this->_listening);
	}

	void	ListeningSocket::start_listening(void)
	{
		this->_listening = listen(this->get_sock(), this->_backlog);
	}

}