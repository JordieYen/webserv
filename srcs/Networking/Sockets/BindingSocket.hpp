#ifndef BIDINGSOCKET_HPP
# define BIDINGSOCKET_HPP

# include <stdio.h>
# include "SimpleSocket.hpp"

namespace	ft
{
	class	BindingSocket : public SimpleSocket
	{
		public:
			BindingSocket(int domain, int service, int protocol, int port, u_long interface);

			int	connect_to_network(int sock, struct sockaddr_in address);
	};
}

#endif