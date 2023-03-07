#include "ConnectingSocket.hpp"

namespace	ft
{
	ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
	{
		this->_connecting = this->connect_to_network(get_sock(), get_address());
		this->test_connection(this->_connecting);
	}

	ConnectingSocket::~ConnectingSocket() {}

	int	ConnectingSocket::connect_to_network(int sock, sockaddr_in address)
	{
		return (connect(sock, (struct sockaddr *)&address, sizeof(address)));
	}

	int	ConnectingSocket::get_connecting(void)
	{
		return (this->_connecting);
	}
}