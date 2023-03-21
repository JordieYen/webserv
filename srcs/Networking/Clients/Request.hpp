#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <cstdlib>

# include <iostream>
# include <sstream>
# include <map>
# include <unistd.h>
# include <sys/socket.h>

using std::string;
using std::istringstream;
using std::map;
using std::make_pair;

namespace ft
{
	class Request
	{	
		public:
			Request(int fd);
			~Request(void);
		
			void	read_buffer(void);
			void	parse_buffer(void);
			void	clear_buffer(void);

			int		get_client_fd(void) const;
			string	get_header(string key) const;
			bool	received(void) const;

		private:
			int					_client_fd;
			string				_content;
			map<string, string>	_headers;

			bool				_received;
	};
}

#endif