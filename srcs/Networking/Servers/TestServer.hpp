#ifndef TESTSERVER_HPP
# define TESTSERVER_HPP

# include <stdio.h>
# include <unistd.h>
# include "SimpleServer.hpp"

namespace	ft
{
	class	TestServer : public SimpleServer
	{
		public:
			TestServer();

			void	launch(void);

		private:
			void	accepter(void);
			void	handler(void);
			void	responder(void);

			std::string	_content;
			char		*_buffer;
			int			_new_socket;
	};
}

#endif