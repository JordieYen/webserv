#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <cstdlib>
# include <cstring>

# include <iostream>
# include <sstream>
# include <fstream>
# include <map>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>

using std::string;
using std::ifstream;
using std::stringstream;
using std::istringstream;
using std::map;
using std::vector;
using std::make_pair;

namespace ft
{
	class Request
	{	
		public:
			Request(int fd);
			~Request(void);
		
			void	validate_content(void);

			void	read_buffer(void);
			void	parse_buffer(void);
			void	clear_buffer(void);

			int		get_client_fd(void) const;
			string	get_header(string key) const;
			bool	received(void) const;

		private:
			void	set_bad_request(void);

			int					_client_fd;
			string				_content;
			string				_body;
			string				_method;
			string				_path;
			map<string, string>	_content_context;

			bool				_received;
	};
}

#endif