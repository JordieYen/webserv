#include "SimpleSocket.hpp"

namespace ft
{
	SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
	{
		this->address.sin_family = domain;
		this->address.sin_port = htons(port);
		this->address.sin_addr.s_addr = htonl(interface);
		this->sock = socket(domain, service, protocol);
		this->test_connection(sock);
		this->connection = this->connect_to_network(this->sock, this->address);
		this->test_connection(connection);
	}

	void	SimpleSocket::test_connection(int item_to_test)
	{
		if (item_to_test < 0)
		{
			perror("Failed to connect...");
			exit(EXIT_FAILURE);
		}
	}

	int	SimpleSocket::get_sock(void)
	{
		return (this->sock);
	}

	int	SimpleSocket::get_connection(void)
	{
		return (this->connection);
	}

	struct sockaddr_in	SimpleSocket::get_address(void)
	{
		return (this->address);
	}
}