#include "ListeningSocket.hpp"

namespace ft
{
	ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
	{
		this->_backlog = backlog;
		this->start_listening();
		test_connection(this->_listening);
	}

	ListeningSocket::ListeningSocket(const ListeningSocket &listeningsocket_clone) : BindingSocket(listeningsocket_clone)
	{
		this->_backlog = listeningsocket_clone._backlog;
		this->_listening = listeningsocket_clone._listening;
	}

	ListeningSocket::~ListeningSocket() {}

	void	ListeningSocket::start_listening(void)
	{
		this->_listening = listen(this->get_sock(), this->_backlog);
	}

}