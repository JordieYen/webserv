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

	SimpleSocket::SimpleSocket(const SimpleSocket &simplesocket_clone)
	{
		this->_address = simplesocket_clone._address;
		// this->_address.sin_family = simplesocket_clone._address.sin_family;
		// this->_address.sin_port = simplesocket_clone._address.sin_port;
		// this->_address.sin_addr.s_addr = simplesocket_clone._address.sin_addr.s_addr;
		this->_sock = simplesocket_clone._sock;
	}

	SimpleSocket::~SimpleSocket()
	{
		close(this->_sock);
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

	struct sockaddr_in	SimpleSocket::get_address(void)
	{
		return (this->_address);
	}

	int	SimpleSocket::get_sock(void) const
	{
		return (this->_sock);
	}

	struct sockaddr_in	SimpleSocket::get_address(void) const
	{
		return (this->_address);
	}
}