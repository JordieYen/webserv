#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <bits/stdc++.h>

# include <iostream>
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
using std::istringstream;

namespace	ft
{
	class Response
	{
		public:
			Response(ServerConfig& config, Request& request);
			~Response(void);

			void	handle_methods(void);

			void	handle_get();
			void	handle_post();
			void	handle_delete();

			void	read_config(string file_name);
			string	get_path_to_index(void);
			string	get_closest_match(void);

		private:
			ServerConfig&	_config;
			Request&		_request;

			string			_index;
	};
}

#endif