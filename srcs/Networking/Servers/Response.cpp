#include "Response.hpp"

namespace ft
{
	Response::Response(ServerConfig& config, Request& request)
	{
		std::cout << "method: " << request.get_header("method") << std::endl;
		std::cout << "path: " << request.get_header("path") << std::endl;
		std::cout << "host: " << request.get_header("host") << std::endl;

		this->_config = config;
		this->_request = request;
		if (request.get_header("method") == "GET")
			handle_get();
		else if (request.get_header("method") == "POST")
			handle_post();
		else if (request.get_header("method") == "DELETE")
			handle_delete();
	}

	Response::~Response(void) {}

	void	Response::handle_get(void)
	{
		// decide on response structure and start implementation
		// if (request.get_header("path") == "/")
		// {

		// }
	}
}