#include "ConfigParser.hpp"

namespace	ft
{
	ConfigParser::ConfigParser(string config_path)
	{
		this->_config_path = config_path;
	}

	vector<ConfigParser::configType>	ConfigParser::get_server_configs(void) const
	{
		return (this->_server_configs);
	}

	vector<ServerConfig>	ConfigParser::get_parsed_server_configs(void) const
	{
		return (this->_parsed_server_configs);
	}

	ConfigParser::lineType	ConfigParser::split_line(string line)
	{
		istringstream	line_stream_space(line);
		string			token_space;
		string			token_tab;
		lineType		line_after_split;

		if (!line.empty() && line.find_first_not_of(" \t") != string::npos)
		{
			while (getline(line_stream_space, token_space, ' '))
			{
				istringstream	line_stream_tab(token_space);
				while (getline(line_stream_tab, token_tab, '\t'))
				{
					if (!token_tab.empty())
						line_after_split.push_back(token_tab);
				}
			}
		}
		return (line_after_split);
	}

	void	ConfigParser::read_config()
	{
		string		line;
		ifstream	config_file(this->_config_path.c_str());
		lineType	current_line;
		configType	current_config;
		int			curl_count = 0;

		if (config_file.is_open())
		{
			while (getline(config_file, line))
			{
				if (line.find("#") != string::npos)
					line = line.substr(0, line.find("#"));
				current_line = split_line(line);
				if (!current_line.empty())
				{
					curl_count += (line.find("{") != string::npos);
					curl_count -= (line.find("}") != string::npos);
					if (!current_config.empty() && current_line.front() == "server" && curl_count == 1)
					{
						this->_server_configs.push_back(current_config);
						current_config.clear();
					}
					current_config.push_back(current_line);
				}
			}
			this->_server_configs.push_back(current_config);
			config_file.close();
		}
		else
			std::cout << "Error: unable to open \"" + this->_config_path + "\"...\n";
	}

	void	ConfigParser::check_config(void)
	{
		for (vector<configType>::iterator config = this->_server_configs.begin(); config != this->_server_configs.end(); config++)
		{
			try 
			{
				this->_error_checker.check_server(*config);
			}
			catch (ErrorChecker::InvalidConfigException & e) 
			{
				std::cout << e.what() << std::endl;
				exit(2);
			}
		}
	}

	void	ConfigParser::parse_config(void)
	{
		for (vector<configType>::iterator config = this->_server_configs.begin(); config != this->_server_configs.end(); config++)
		{
			ServerConfig	new_config;

			for (configType::iterator line = config->begin() + 1; line != config->end(); line++)
			{
				if (line->front() == "location")
				{
					LocationBlock	new_location(line->at(1) == "=");
					string			path = line->at(1 + (line->at(1) == "="));

					while ((++line)->front() != "}")
					{
						if (line->back() == ";")
							line->pop_back();
						else
							line->back() = line->back().substr(0, line->back().length() - 1);
						new_location.set_directive(line->front(), LocationBlock::valueType(line->begin() + 1, line->end()));
					}
					new_config.set_location_directive(path, new_location);
				}
				else
				{
					if (line->back() == ";")
						line->pop_back();
					else
						line->back() = line->back().substr(0, line->back().length() - 1);
					new_config.set_normal_directive(line->front(), ServerConfig::normalValueType(line->begin() + 1, line->end()));
				}
			}
			this->_parsed_server_configs.push_back(new_config);
		}
	}
}