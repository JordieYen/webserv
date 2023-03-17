#ifndef RESPONSE_HPP
# define RESPONSE_HPP

// # include <bits/stdc++.h>

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <sstream>
# include <fstream>
# include "ServerConfig.hpp"
# include "../Clients/Request.hpp"

using std::string;
using std::vector;
using std::map;
using std::make_pair;
using std::ifstream;
using std::stringstream;

namespace	ft
{
	class Response
	{
		public:
			Response(ServerConfig& config, Request& request);
			~Response(void);

			string	get_content_length(void);
			
			string	get_closest_match(void);
			string	get_path_to_index(void);
			void	read_config(string file_name);

			void	prepend_header(void);

			void	handle_get();
			// void	handle_post();
			// void	handle_delete();
			void	handle_methods(void);

		private:
			ServerConfig&	_config;
			Request&		_request;

			int				_status_code;
			
			string			_content;
	};
}

#endif