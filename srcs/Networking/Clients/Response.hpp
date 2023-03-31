#ifndef RESPONSE_HPP
# define RESPONSE_HPP

// # include <bits/stdc++.h>

# include <iostream>
# include <string>
# include <cstring>
# include <ctime>
# include <cstdio>
# include <sstream>
# include <fstream>
# include <fcntl.h>
# include <dirent.h>
# include <random>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "../Servers/SimpleServer.hpp"
# include "../Servers/ServerConfig.hpp"
# include "Request.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::stringstream;

namespace	ft
{
	class Response
	{
		public:
			Response(SimpleServer& server, Request* request);
			~Response(void);

			bool	path_is_valid_file(string path);
			
			string	get_string_from_numeral(int	input);
			string	get_status_message(void);
			
			string	get_closest_match(void);
			string	get_path_to(string directive);
			string	get_path_to_index(void);
			string	get_path_to_file(void);
			string	get_path_to_error(void);

			string	generate_random_hash(void);
			string	generate_time_limit(int	minutes);

			void	append_icons(void);
			void	handle_autoindex(string line);
			void	read_file(string file_name);

			bool	handle_return(void);
			void	prepend_header(void);
			void	send_to_client(void);

			bool	check_error(void);
			void	handle_error(void);

			bool	check_cgi(void);
			void	handle_cgi(void);

			void	handle_get(void);
			void	handle_post(void);
			void	handle_delete(void);
			void	handle_methods(void);

			bool	sent(void);

		private:
			SimpleServer&	_server;
			ServerConfig&	_config;
			Request*		_request;

			string			_closest_match;
			string			_root;
			string			_username;
			bool			_is_autoindex;

			int				_status_code;
			string			_content;
			bool			_sent;
	};
}

#endif