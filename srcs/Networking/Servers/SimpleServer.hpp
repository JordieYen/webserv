#ifndef SIMPLESERVER_HPP
# define SIMPLESERVER_HPP

# include <cstdlib>

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
			~SimpleServer();

			ServerConfig&					get_config(void);
			int								get_port(void) const;
			ListeningSocket*				get_socket(void) const;
			int								get_server_fd(void) const;
			map<string, vector<string> >*	get_cookies(void) const;

			int								accept_connection(void);

			bool							insert_cookie(string key, string value);

		private:
			ServerConfig					_config;
			int								_port;
			ListeningSocket*				_socket;

			map<string, vector<string> >*	_cookies;
	};
}

#endif