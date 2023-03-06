#include "ListeningSocket.hpp"

namespace ft
{
	ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
	{
		this->_backlog = backlog;
		this->start_listening();
		std::cout << "3" << std::endl;
		test_connection(this->_listening);
	}

	void	ListeningSocket::start_listening(void)
	{
		std::cout << "1" << std::endl;
		std::cout << this->get_connection() << std::endl;
		this->_listening = listen(this->get_connection(), this->_backlog);
		std::cout << "2" << std::endl;
	}

}