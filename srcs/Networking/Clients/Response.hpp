#ifndef RESPONSE_HPP
# define RESPONSE_HPP

// # include <bits/stdc++.h>

# include <iostream>
# include <string>
# include <cstring>
# include <cstdio>
# include <sstream>
# include <fstream>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/socket.h>
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
			Response(ServerConfig& config, Request* request);
			~Response(void);

			bool	path_is_valid_file(string path);
			
			string	get_content_length(void);
			string	get_status_message(void);
			
			string	get_closest_match(void);
			string	get_path_to(string directive, string match);
			string	get_path_to_index(string root, string match);
			string	get_path_to_file(void);

			void	append_icons(void);
			void	handle_autoindex(string line);
			void	read_file(string file_name);

			bool	handle_return(void);
			void	prepend_header(void);
			void	send_to_client(void);

			bool	check_error(void);
			void	handle_error(void);

			void	handle_get(void);
			void	handle_post(void);
			void	handle_delete(void);
			void	handle_methods(void);

			bool	sent(void);

		private:
			ServerConfig&	_config;
			Request*		_request;

			string			_closest_match;
			string			_root;
			bool			_is_autoindex;

			int				_status_code;
			string			_content;
			bool			_sent;
	};
}

#endif