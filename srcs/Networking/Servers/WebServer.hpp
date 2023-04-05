#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

// # include <iostream>
# include <vector>
# include <map>
# include <poll.h>
# include <signal.h>
# include "SimpleServer.hpp"
# include "../../Parsing/Parsing.hpp"
# include "../Clients/Clients.hpp"

# define DEFAULT_CONFIG_PATH "default.conf"

using std::string;
using std::vector;
using std::map;
using std::make_pair;

namespace	ft
{
	class WebServer
	{
		public:
			typedef struct pollfd						pollFdType;
			typedef map<int, SimpleServer*>				serverMapType;
			typedef map<int, SimpleServer*>::iterator	serverIterType;
			typedef vector<Client*>						clientArrayType;
			typedef vector<Client*>::iterator			clientIterType;
			typedef vector<pollFdType>					pollFdArrayType;

			WebServer(void);
			WebServer(string config_path);
			~WebServer(void);

			void	setup(void);
			void	launch(void);

		private:
			bool			current_pollfd_is(int event);
			void			set_current_pollfd_to(int event);

			void			accept_connection(serverIterType server);
			clientIterType	close_connection(clientIterType client);

			ConfigParser	_parser;
			serverMapType	_servers;
			clientArrayType	_clients;
			pollFdArrayType	_pollfds;
			int				_current_pollfd_index;
	};
}

#endif