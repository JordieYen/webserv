#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <cstdlib>

# include <fstream>
# include <sstream>
# include <unistd.h>
# include <sys/types.h>
# include <poll.h>
# include "ServerConfig.hpp"
# include "../Sockets/Sockets.hpp"
# include "../Clients/Request.hpp"
# include "Response.hpp"

using std::string;
using std::ifstream;
using std::istringstream;

namespace	ft
{
	class	SimpleServer
	{
		public:
			typedef struct pollfd	pollFdType;
			SimpleServer(ServerConfig config);
			~SimpleServer();

			int					get_port(void) const;
			ListeningSocket*	get_socket(void) const;
			int					get_server_fd(void) const;
			int					get_pollfd_index(void) const;
			
			void				set_pollfd_index(int pollfd_index);

			int		accept_connection(void);
			// void	handler(void);
			void	respond(Request& request);

		private:
			ServerConfig		_config;
			int					_port;
			ListeningSocket*	_socket;
			int					_pollfd_index;
	};
}

#endif