#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <cstdlib>
# include <sstream>
# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include "ServerConfig.hpp"
# include "../Sockets/Sockets.hpp"

namespace	ft
{
	class	SimpleServer
	{
		public:
			SimpleServer(ServerConfig config);
			SimpleServer(const SimpleServer &simpleserver_clone);
			~SimpleServer();

			int				get_port(void) const;
			int				get_server_fd(void) const;
			ListeningSocket	*get_socket(void) const;

			void	accepter(void);
			void	handler(void);
			void	responder(void);

		private:
			ServerConfig	_config;
			ListeningSocket	*_socket;
			int				_port;

			std::string		_content;
			char			*_buffer;
			int				_new_socket;
	};
}

#endif