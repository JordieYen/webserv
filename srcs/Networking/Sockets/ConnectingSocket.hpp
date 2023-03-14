#ifndef CONNECTINGSOCKET_HPP
# define CONNECTINGSOCKET_HPP

# include "SimpleSocket.hpp"

namespace	ft
{
	class	ConnectingSocket : public SimpleSocket
	{
		public:
			ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
			~ConnectingSocket();

			int	connect_to_network(int sock, struct sockaddr_in address);
	};
}

#endif