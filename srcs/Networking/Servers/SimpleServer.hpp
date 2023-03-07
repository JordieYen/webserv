#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include "../Sockets/Sockets.hpp"

namespace	ft
{
	class	SimpleServer
	{
		public:
			SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog);
			~SimpleServer();

			ListeningSocket	*get_socket(void);

			void	launch(void);

		private:
			void	accepter(void);
			void	handler(void);
			void	responder(void);

			ListeningSocket	*_socket;
			std::string		_content;
			char			*_buffer;
			int				_new_socket;
	};
}

#endif