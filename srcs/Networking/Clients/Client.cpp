#include "Client.hpp"

namespace	ft
{
	Client::Client(int fd) : _request(fd)
	{
		this->_fd = fd;
	}

	Client::~Client() {};

	int	Client::get_fd(void) const
	{
		return (this->_fd);
	}

	Request&	Client::get_request(void)
	{
		return (this->_request);
	}

	int	Client::get_pollfd_index(void) const
	{
		return (this->_pollfd_index);
	}

	void	Client::set_pollfd_index(int pollfd_index)
	{
		this->_pollfd_index = pollfd_index;
	}

	void	Client::read_buffer(void)
	{
		this->_request.read_buffer();
	}
}
