#include "TestServer.hpp"

namespace	ft
{
	TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
	{
		launch();
	}

	void	TestServer::accepter(void)
	{
		struct sockaddr_in	address = this->get_socket()->get_address();
		int					addrlen = sizeof(address);

		this->_new_socket = accept(this->get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
		this->_buffer = static_cast<char *>(malloc(30000 * sizeof(char)));
		read(this->_new_socket, this->_buffer, 30000);
		this->_content = std::string(this->_buffer);
		free(this->_buffer);
	}

	void	TestServer::handler(void)
	{
		std::cout << this->_content << std::endl;
	}

	void	TestServer::responder(void)
	{
		write(this->_new_socket, "Hello from server!", 19);
		close(this->_new_socket);
	}

	void	TestServer::launch(void)
	{
		while (true)
		{
			std::cout << "Waiting...\n";
			this->accepter();
			this->handler();
			this->responder();
			std::cout << "Done!!!\n";
		}
	}
}