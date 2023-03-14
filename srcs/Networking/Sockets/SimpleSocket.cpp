#include "SimpleSocket.hpp"

namespace ft
{
	SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface)
	{
		int	option = 1;

		this->_address.sin_family = domain;
		this->_address.sin_port = htons(port);
		this->_address.sin_addr.s_addr = htonl(interface);
		this->_fd = socket(domain, service, protocol);
		setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
		fcntl(this->_fd, F_SETFL, fcntl(this->_fd, F_GETFL, 0) | O_NONBLOCK);
		this->test_connection(this->_fd);
	}

	SimpleSocket::~SimpleSocket()
	{
		close(this->_fd);
	}

	void	SimpleSocket::test_connection(int item_to_test)
	{
		if (item_to_test < 0)
		{
			perror("Failed to connect...");
			exit(EXIT_FAILURE);
		}
	}

	int	SimpleSocket::get_fd(void) const
	{
		return (this->_fd);
	}

	struct sockaddr_in	SimpleSocket::get_address(void) const
	{
		return (this->_address);
	}
}