#include "BindingSocket.hpp"

namespace	ft
{
	BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
	{
		this->_binding = this->connect_to_network(get_sock(), get_address());
		this->test_connection(this->_binding);
	}

	BindingSocket::BindingSocket(const BindingSocket &bindingsocket_clone) : SimpleSocket(bindingsocket_clone)
	{
		this->_binding = bindingsocket_clone._binding;
	}

	BindingSocket::~BindingSocket() {}

	int	BindingSocket::connect_to_network(int sock, sockaddr_in address)
	{
		return (bind(sock, (struct sockaddr *)&address, sizeof(address)));
	}

	int	BindingSocket::get_binding(void)
	{
		return (this->_binding);
	}
}