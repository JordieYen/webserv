#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <iostream>
# include <vector>
# include <map>
# include "SimpleServer.hpp"
# include "../../Parsing/Parsing.hpp"

namespace	ft
{
	class WebServer
	{
		public:
			typedef std::vector<SimpleServer>	serverArrayType;
			typedef std::vector<int>			fdArrayType;

			WebServer(void);
			WebServer(std::string input_file);
			~WebServer(void);

			void	launch(void);

		private:
			ConfigParser	parser;
			fdArrayType		_fds;
			serverArrayType	_servers;
	};
}

#endif