#ifndef ERRORCHECKER_HPP
# define ERRORCHECKER_HPP

# include <stdio.h>
# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <iterator>
# include <algorithm>

namespace ft
{
	class ErrorChecker
	{
		public:
			typedef std::string						value_type;
			typedef	std::vector<value_type>		line_type;
			typedef	std::vector<line_type>			config_type;

			ErrorChecker(void);
			~ErrorChecker(void);

			void	check_server(config_type block_to_check);

			class InvalidConfigException: public std::exception
			{
				public:
					InvalidConfigException(std::string	error_message)
					{
						this->_error = error_message;
					}

					~InvalidConfigException() throw() {}

					virtual const char* what() const throw() // understand better
					{
						return (this->_error.c_str());
					}
				private:
					std::string	_error;
			};

		private:
			void	check_line(config_type::iterator& line, size_t* open_curl_count, size_t* close_curl_count);
			void	check_token(line_type::iterator token, config_type::iterator& line, std::vector<char>::iterator& check);
	};
}

#endif