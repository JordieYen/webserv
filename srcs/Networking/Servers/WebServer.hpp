#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include "SimpleServer.hpp"
# include "../../Parsing/Parsing.hpp"

# define DEFAULT_CONFIG_PATH "default.conf"

namespace	ft
{
	class WebServer
	{
		public:
			typedef std::map<int, SimpleServer>	serverMapType;
			typedef std::vector<int>			fdArrayType;

			WebServer(void);
			WebServer(std::string config_path);
			~WebServer(void);

			void	setup(void);
			void	launch(void);

		private:
			ConfigParser	_parser;
			fdArrayType		_client_fds;
			serverMapType	_servers;
	};
}

#endif