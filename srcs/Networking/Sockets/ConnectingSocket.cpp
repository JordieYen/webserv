#include "ConnectingSocket.hpp"

namespace	ft
{
	ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface)
	{
		this->test_connection(this->connect_to_network(get_fd(), get_address()));
	}

	ConnectingSocket::~ConnectingSocket() {}

	int	ConnectingSocket::connect_to_network(int sock, sockaddr_in address)
	{
		return (connect(sock, (struct sockaddr *)&address, sizeof(address)));
	}
}