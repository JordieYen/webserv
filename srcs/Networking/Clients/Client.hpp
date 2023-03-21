#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "../Servers/ServerConfig.hpp"
# include "Request.hpp"
# include "Response.hpp"

namespace	ft
{
	class	Client
	{
		public:
			Client(ServerConfig& config, int fd);
			~Client();

			bool	received_request(void);
			bool	sent_response(void);

			void	handle_request(void);
			void	handle_response(void);

		private:
			Request*	_request;
			Response*	_response;
	};
}

#endif