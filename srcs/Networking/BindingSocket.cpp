#include "BindingSocket.hpp"

namespace	ft
{
	BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
	{
		this->set_connection(this->connect_to_network(get_sock(), get_address()));
		this->test_connection(this->get_connection());
	}

	int	BindingSocket::connect_to_network(int sock, sockaddr_in address)
	{
		return (bind(sock, (struct sockaddr *)&address, sizeof(address)));
	}
}