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

			ErrorChecker(config_type block_to_check);
			~ErrorChecker(void);

		private:
			void	check_line(config_type::iterator& line, size_t* open_curl_count, size_t* close_curl_count);
			void	check_token(line_type::iterator token, config_type::iterator& line, std::vector<char>::iterator& check);

			bool	_is_valid;
	};
}

#endif