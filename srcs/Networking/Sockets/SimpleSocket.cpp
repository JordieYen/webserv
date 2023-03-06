#include "SimpleSocket.hpp"

namespace ft
{
	SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
	{
		this->_address.sin_family = domain;
		this->_address.sin_port = htons(port);
		this->_address.sin_addr.s_addr = htonl(interface);
		this->_sock = socket(domain, service, protocol);
		this->test_connection(this->_sock);
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
		return (this->_sock);
	}

	int	SimpleSocket::get_connection(void)
	{
		return (this->_connection);
	}

	struct sockaddr_in	SimpleSocket::get_address(void)
	{
		return (this->_address);
	}

	void	SimpleSocket::set_connection(int connection)
	{
		this->_connection = connection;
	}
}