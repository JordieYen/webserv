#include "Client.hpp"

namespace	ft
{
	Client::Client(SimpleServer& server, int fd)
	{
		this->_request = new Request(fd);
		this->_response = new Response(server, this->_request);
	}

	Client::~Client()
	{
		delete this->_request;
		delete this->_response;
	};

	bool	Client::received_request(void)
	{
		return (this->_request->received());
	}

	bool	Client::sent_response(void)
	{
		return (this->_response->sent());
	}

	void	Client::handle_request(void)
	{
		this->_request->handle_request();
	}

	void	Client::handle_response(void)
	{
		this->_response->handle_methods();
	}
}
