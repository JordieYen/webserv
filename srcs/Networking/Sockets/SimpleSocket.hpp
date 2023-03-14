#ifndef SIMPLESOCKET_HPP
# define SIMPLESOCKET_HPP

# include <cstdlib>
# include <stdio.h>

# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

namespace ft
{
	class SimpleSocket
	{
		public:
			SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
			virtual	~SimpleSocket();

			virtual int	connect_to_network(int sock_fd, struct sockaddr_in address) = 0;
			void		test_connection(int);

			int					get_fd(void) const;
			struct sockaddr_in	get_address(void) const;
	
		private:
			int					_fd;
			struct sockaddr_in	_address;
	};
}

#endif