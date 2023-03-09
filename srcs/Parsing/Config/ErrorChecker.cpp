#include "ErrorChecker.hpp"


namespace ft
{
	ErrorChecker::ErrorChecker(void) {}

	ErrorChecker::~ErrorChecker(void) {}

	void	ErrorChecker::check_server(config_type block_to_check)
	{
		size_t				open_curl_count = 0;
		size_t				close_curl_count = 0;

		if (block_to_check[0].front() != "server" || block_to_check[0].size() != 2)
			throw ErrorChecker::InvalidConfigException("Error : server not found");
		for (std::vector<std::vector<std::string> >::iterator line = block_to_check.begin(); line != block_to_check.end(); line++)
		{
			this->check_line(line, &open_curl_count, &close_curl_count);
		}
		if (open_curl_count != close_curl_count)
		{
			if (open_curl_count > close_curl_count)
				throw ErrorChecker::InvalidConfigException("Error : missing '}' ");
			else
				throw ErrorChecker::InvalidConfigException("Error : missing '{' ");
		}
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
		if (*check == '}' && (&(*line).back() != &(*line).front() || *token != "}"))
			throw ErrorChecker::InvalidConfigException(error);
		if (*check == ';' && (&(*token) != &((*line).back()) || *((*token).end() - 1) != ';'))
			throw ErrorChecker::InvalidConfigException(error);
	}

}