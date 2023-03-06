#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <stdio.h>
# include "../ftlibc-networking.hpp"

namespace	ft
{
	class	SimpleServer
	{
		public:
			SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog);
			~SimpleServer();

			virtual void	launch(void) = 0;

			ListeningSocket	*get_socket(void);

		private:
			virtual void	accepter(void) = 0;
			virtual void	handler(void) = 0;
			virtual void	responder(void) = 0;

			ListeningSocket	*_socket;
	};
}

#endif