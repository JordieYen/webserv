#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <stdio.h>
# include <iostream>
# include <sstream>
# include <fstream>
# include <vector>
# include "../../Networking/Servers/ServerConfig.hpp"
# include "ErrorChecker.hpp"

using std::string;
using std::ifstream;
using std::istringstream;
using std::vector;
using std::getline;

namespace	ft
{
	class	ConfigParser
	{
		public:
			typedef vector<string>		lineType;
			typedef vector<lineType>	configType;

			ConfigParser(string config_path = "");

			vector<configType>		get_server_configs(void) const;
			vector<ServerConfig>	get_parsed_server_configs(void) const;

			void		read_config(void);
			void		check_config(void);
			void		parse_config(void);

		private:
			lineType				split_line(string line);

			ErrorChecker			_error_checker;
			string					_config_path;
			vector<configType>		_server_configs;
			vector<ServerConfig>	_parsed_server_configs;
	};
}

#endif