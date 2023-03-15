#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <vector>
# include <map>
# include "ServerConfig.hpp"
# include "../Clients/Request.hpp"

using std::string;
using std::vector;
using std::map;
using std::make_pair;

namespace	ft
{
	class Response
	{
		public:
			Response(ServerConfig& config, Request& request);
			~Response(void);

			void	handle_get();
			void	handle_post();
			void	handle_delete();

		private:
			ServerConfig&	_config;
			Request&		_request;
	};
}

#endif