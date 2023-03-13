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
			BindingSocket(const BindingSocket &bindingsocket_clone);
			~BindingSocket();

			int	connect_to_network(int sock, struct sockaddr_in address);

			int	get_binding(void);

		private:
			int	_binding;
	};
}

#endif