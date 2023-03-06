# include "ListeningSocket.hpp"

int	main(void)
{
	std::cout << "Starting..." << std::endl;
	std::cout << "Binding Socket..." << std::endl;
	ft::BindingSocket bs = ft::BindingSocket(AF_INET, SOCK_STREAM, 0, 81, INADDR_ANY);
	std::cout << "Listening Socket..." << std::endl;
	ft::ListeningSocket ls = ft::ListeningSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
	std::cout << "Susccess!" << std::endl;
}