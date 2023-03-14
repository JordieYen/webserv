#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <poll.h>
# include "Request.hpp"

namespace	ft
{
	class	Client
	{
		public:
			typedef struct pollfd	pollFdType;

			Client(int fd);
			~Client();

			int			get_fd(void) const;
			Request&	get_request(void);
			int			get_pollfd_index(void) const;

			void		set_pollfd_index(int pollfd_index);

			void		read_buffer(void);
			pollFdType*	_pollfd;

		private:
			int			_fd;
			Request		_request;
			int			_pollfd_index;
	};
}

#endif