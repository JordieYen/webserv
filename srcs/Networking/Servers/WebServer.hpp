#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include <poll.h>
# include "SimpleServer.hpp"
# include "../../Parsing/Parsing.hpp"
# include "../Clients/Clients.hpp"

# define DEFAULT_CONFIG_PATH "default.conf"

using std::string;
using std::vector;
using std::map;
using std::make_pair;
using std::cout; //NEVER REMOVE ASSHOLES
using std::endl; //NEVER REMOVE ASSHOLES

namespace	ft
{
	class WebServer
	{
		public:
			typedef struct pollfd			pollFdType;
			typedef map<int, SimpleServer*>	serverMapType;
			typedef map<int, Client*>		clientMapType;
			typedef vector<pollFdType>		pollFdArrayType;

			WebServer(void);
			WebServer(string config_path);
			~WebServer(void);

			void	setup(void);
			void	launch(void);

		private:
			ConfigParser	_parser;
			pollFdArrayType	_pollfds;
			serverMapType	_servers;
			clientMapType	_clients;
	};
}

#endif