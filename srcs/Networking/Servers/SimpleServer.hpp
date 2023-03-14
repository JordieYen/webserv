#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <fstream>
# include <sstream>
# include <unistd.h>
# include <sys/types.h>
# include "ServerConfig.hpp"
# include "../Sockets/Sockets.hpp"

using std::string;
using std::ifstream;
using std::istringstream;

namespace	ft
{
	class	SimpleServer
	{
		public:
			SimpleServer(ServerConfig config);
			SimpleServer(const SimpleServer &simpleserver_clone);
			~SimpleServer();

			int					get_port(void) const;
			int					get_server_fd(void) const;
			ListeningSocket*	get_socket(void) const;

			void	accepter(void);
			void	handler(void);
			void	responder(void);

		private:
			ServerConfig		_config;
			ListeningSocket*	_socket;
			int					_port;

			std::string			_content;
			char				*_buffer;
			int					_new_socket;
	};
}

#endif