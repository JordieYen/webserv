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
			
			string	get_header(string key);
			bool	has_read(void) const;

		private:
			int					_client_fd;
			string				_content;
			map<string, string>	_headers;
			bool				_has_read;
	};
}

#endif