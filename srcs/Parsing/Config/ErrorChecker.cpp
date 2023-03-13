#include "ErrorChecker.hpp"


namespace ft
{
	ErrorChecker::ErrorChecker(void) {}

	ErrorChecker::~ErrorChecker(void) {}

	void	ErrorChecker::check_server(config_type block_to_check)
	{
		size_t				open_curl_count = 1;
		size_t				close_curl_count = 0;

		if (block_to_check.size() < 3)
			throw ErrorChecker::InvalidConfigException("Error : invalid syntax ");
		if (block_to_check.front().front() != "server" || block_to_check.front().back() != "{" || block_to_check.front().size() != 2)
			throw ErrorChecker::InvalidConfigException("Error : server not found ");
		for (std::vector<std::vector<std::string> >::iterator line = block_to_check.begin() + 1; line != block_to_check.end(); line++)
		{
			if (!(*line).empty())
			{
				this->check_line(line, &open_curl_count, &close_curl_count);
				if (((*line).size() < 2 && (*line).front() != "}") || ((*line).size() < 3 && (*line).back() == ";"))
					throw ErrorChecker::InvalidConfigException("Error : missing value for key ");
				this->check_key(line);
			}
		}
		if (open_curl_count != close_curl_count)
		{
			if (open_curl_count > close_curl_count)
				throw ErrorChecker::InvalidConfigException("Error : missing '}' ");
			else
				throw ErrorChecker::InvalidConfigException("Error : missing '{' ");
		}
	}

	void	ErrorChecker::check_key(config_type::iterator& line)
	{
		const char							*key_checks[] = {"listen", "server_name", "autoindex", "access_log", "root", "index", "return", "location", "}"};
		std::vector<std::string>			checks(key_checks, key_checks + 9);
		bool								is_valid = false;
		static bool							is_location_block;
		
		for (std::vector<std::string>::iterator check = checks.begin(); check != checks.end(); check++)
		{
			if (is_location_block == true && (*line).front() == "}")
				is_location_block = false;
			if (is_location_block == true)
			{
				this->check_location_key(line);
				is_valid = true;
				break;
			}
			if ((*line).front() == *check)
			{
				is_valid = true;
				if (*check == "location")
				{
					is_location_block = true;
					if (line->size() < 3 || (line + 1)->front() == "}")
						throw ErrorChecker::InvalidConfigException("Error : invalid location block syntax ");
				}
				break;
			}
		}
		if (is_valid == false)
			throw ErrorChecker::InvalidConfigException("Error : '" + (*line).front() + "' is an invalid key ");
	}

	void	ErrorChecker::check_line(config_type::iterator& line, size_t* open_curl_count, size_t* close_curl_count)
	{
		const char			testchecks[] = { '{', '}', ';' };
		std::vector<char>	checks(testchecks, testchecks + 3);
		size_t				found;

		for (std::vector<std::string>::iterator token = (*line).begin(); token != (*line).end(); token++)
		{
			*open_curl_count += std::count((*token).begin(), (*token).end(), '{');
			*close_curl_count += std::count((*token).begin(), (*token).end(), '}');
			for (std::vector<char>::iterator check = checks.begin(); check != checks.end(); check++)
			{
				found = (*token).find(*check);
				if (found != std::string::npos)
				{
					check_token(token, line, check);
					break;
				}
			}
			if (found != std::string::npos)
				break;
		}
		if (found == std::string::npos)
			throw ErrorChecker::InvalidConfigException("Error : missing '{' '}' ';' at end of line ");
	}

	void	ErrorChecker::check_token(line_type::iterator token, config_type::iterator& line, std::vector<char>::iterator& check)
	{
		std::string error;

		error.append("Error : '");
		error.append(std::string(1, *check));
		error.append("' invalid position");
		if (*check == '{' && ((*token != "{" || &(*token) != &((*line).back())) || &(*line).back() == &(*line).front()))
			throw ErrorChecker::InvalidConfigException(error);
		else if (*token == "{")
			this->check_open_curly_bracket(line);
		if (*check == '}' && (&(*line).back() != &(*line).front() || *token != "}"))
			throw ErrorChecker::InvalidConfigException(error);
		if (*check == ';' && (&(*token) != &((*line).back()) || *((*token).end() - 1) != ';'))
			throw ErrorChecker::InvalidConfigException(error);
	}

	void	ErrorChecker::check_open_curly_bracket(config_type::iterator& line)
	{
		const char							*key_checks[] = {"location"};
		std::vector<std::string>			checks(key_checks, key_checks + 1);
		bool								is_valid = false;

		for (std::vector<std::string>::iterator check = checks.begin(); check != checks.end(); check++)
		{
			if ((*line).front() == *check)
			{
				is_valid = true;
				break;
			}
		}
		if (is_valid == false)
			throw ErrorChecker::InvalidConfigException("Error : '" + (*line).front() + "' is an invalid key for start of block");
	}

	void	ErrorChecker::check_location_key(config_type::iterator& line)
	{
		const char							*key_checks[] = {"root", "proxy_pass", "expires"};
		std::vector<std::string>			checks(key_checks, key_checks + 3);
		bool								is_valid = false;

		for (std::vector<std::string>::iterator check = checks.begin(); check != checks.end(); check++)
		{
			if ((*line).front() == *check)
			{
				is_valid = true;
				break;
			}
		}
		if (is_valid == false)
			throw ErrorChecker::InvalidConfigException("Error : '" + (*line).front() + "' is an invalid key for location block");
	}
}