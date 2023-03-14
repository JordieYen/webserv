#ifndef BINDINGSOCKET_HPP
# define BINDINGSOCKET_HPP

# include "SimpleSocket.hpp"

namespace	ft
{
	class	BindingSocket : public SimpleSocket
	{
		public:
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);
			~BindingSocket();

			int	connect_to_network(int sock, struct sockaddr_in address);
	};
}

#endif