#ifndef ERRORCHECKER_HPP
# define ERRORCHECKER_HPP

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <iterator>
# include <algorithm>

using std::string;
using std::istringstream;
using std::vector;
using std::count;

namespace ft
{
	class ErrorChecker
	{
		public:
			typedef string						valueType;
			typedef	vector<valueType>			lineType;
			typedef	vector<lineType>			configType;
			typedef vector<char>				charCheckType;
			typedef vector<string>				stringCheckType;
			typedef vector<valueType>::iterator	lineIterType;
			typedef vector<lineType>::iterator	configIterType;
			typedef vector<char>::iterator		charCheckIterType;
			typedef vector<string>::iterator	stringCheckIterType;

			ErrorChecker(void);
			~ErrorChecker(void);

			void	check_server(configType block_to_check);

		private:
			void	check_location_key(configIterType& line);
			void	check_open_curly_bracket(configIterType& line);
			void	check_key(configIterType& line);
			void	check_line(configIterType& line, size_t* open_curl_count, size_t* close_curl_count);
			void	check_token(lineIterType token, configIterType& conline, charCheckIterType& check);

		public:
			class InvalidConfigException: public std::exception
			{
				public:
					InvalidConfigException(string error_message)
					{
						this->_error = error_message;
					}

					~InvalidConfigException() throw() {}

					virtual const char* what() const throw()
					{
						return (this->_error.c_str());
					}

				private:
					string	_error;
			};
	};
}

#endif