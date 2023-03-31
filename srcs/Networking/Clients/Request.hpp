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

			void	handle_request(void);
			void	clear(void);

			int						get_client_fd(void) const;
			string					get_header(string key) const;
			map<string, string>&	get_body_map(void);
			map<string, string>&	get_files_map(void);
			map<string, string>&	get_cookies_map(void);
			bool					received(void) const;

		private:
			void	read_from_client(void);

			bool	header_not_valid(void);
			void	set_bad_request(void);
			void	parse_header(void);
			void	parse_cookies(string line);

			bool	context_equals(string context, string value);
			bool	body_has_minimum_length(void);
			void	parse_urlencoded_body(void);
			void	parse_multipart_body(void);

			int					_client_fd;
			string				_content;
			map<string, string>	_content_context;

			string				_method;
			string				_path;
			string				_referrer;

			map<string, string>	_body;
			map<string, string>	_files;
			map<string, string>	_cookies;

			bool				_received;
	};
}

#endif
