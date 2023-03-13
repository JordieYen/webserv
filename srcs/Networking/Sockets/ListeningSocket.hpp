#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include <stdio.h>
# include "BindingSocket.hpp"

namespace ft
{
	class ListeningSocket : public BindingSocket
	{
		public:
			ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
			~ListeningSocket();

			void	start_listening(void);
			
		private:
			int	_backlog;
			int	_listening;
	};
}

#endif