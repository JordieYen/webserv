#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP

# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

namespace ft
{
	class SimpleSocket
	{
		public:
			SimpleSocket(int domain, int service, int protocol, int port, u_long interface);

			virtual int	connect_to_network(int sock, struct sockaddr_in address) = 0;
			void		test_connection(int);

			int					get_sock(void);
			int					get_connection(void);
			struct sockaddr_in	get_address(void);
	
		private:
			int					sock;
			int					connection;
			struct sockaddr_in	address;

	};
}

#endif