#include "ErrorChecker.hpp"

namespace ft
{
	ErrorChecker::ErrorChecker(void) {}

	ErrorChecker::~ErrorChecker(void) {}

	void	ErrorChecker::check_server(configType block_to_check)
	{
		size_t	open_curl_count = 1;
		size_t	close_curl_count = 0;

		if (block_to_check.size() < 3)
			throw InvalidConfigException("Error : invalid syntax ");
		if (block_to_check.front().front() != "server" || block_to_check.front().back() != "{" || block_to_check.front().size() != 2)
			throw InvalidConfigException("Error : server not found ");
		for (configIterType line = block_to_check.begin() + 1; line != block_to_check.end(); line++)
		{
			if (!line->empty())
			{
				this->check_line(line, &open_curl_count, &close_curl_count);
				if ((line->size() < 2 && line->front() != "}") || (line->size() < 3 && line->back() == ";"))
					throw InvalidConfigException("Error : missing value for key ");
				this->check_key(line);
			}
		}
		if (open_curl_count != close_curl_count)
		{
			if (open_curl_count > close_curl_count)
				throw InvalidConfigException("Error : missing '}' ");
			else
				throw InvalidConfigException("Error : missing '{' ");
		}
	}

	void	ErrorChecker::check_key(configIterType& line)
	{
		const char*		key_checks[] = {"listen", "root", "index", "return", "location", "}"};
		stringCheckType	checks(key_checks, key_checks + 6);
		bool			is_valid = false;
		static bool		is_location_block;
		
		for (stringCheckIterType check = checks.begin(); check != checks.end(); check++)
		{
			if (is_location_block == true && line->front() == "}")
				is_location_block = false;
			if (is_location_block == true)
			{
				this->check_location_key(line);
				is_valid = true;
				break;
			}
			if (line->front() == *check)
			{
				is_valid = true;
				if (*check == "listen")
				{
					for (lineIterType token = line->begin() + 1; token != line->end(); token++)
					{
						int				port;
						istringstream	port_stream(*token);

						port_stream >> port;
						if (token->find_first_not_of("0123456789;") != string::npos)
							throw InvalidConfigException("Error : invalid port ");
						if (port > pow(2, 16) - 1)
							throw InvalidConfigException("Error : port exceeds limit ");
					}
				}
				if (*check == "location")
				{
					is_location_block = true;
					if (line->size() < 3 || (line + 1)->front() == "}")
						throw InvalidConfigException("Error : invalid location block syntax ");
				}
				break;
			}
		}
		if (is_valid == false)
			throw InvalidConfigException("Error : '" + line->front() + "' is an invalid key ");
	}

	void	ErrorChecker::check_line(configIterType& line, size_t* open_curl_count, size_t* close_curl_count)
	{
		const char		testchecks[] = { '{', '}', ';' };
		charCheckType	checks(testchecks, testchecks + 3);
		size_t			found;

		for (lineIterType token = line->begin(); token != line->end(); token++)
		{
			*open_curl_count += count(token->begin(), token->end(), '{');
			*close_curl_count += count(token->begin(), token->end(), '}');
			for (charCheckIterType check = checks.begin(); check != checks.end(); check++)
			{
				found = token->find(*check);
				if (found != string::npos)
				{
					check_token(token, line, check);
					break;
				}
			}
			if (found != string::npos)
				break;
		}
		if (found == string::npos)
			throw InvalidConfigException("Error : missing '{' '}' ';' at end of line ");
	}

	void	ErrorChecker::check_token(lineIterType token, configIterType& line, charCheckIterType& check)
	{
		string error;

		error.append("Error : '");
		error.append(string(1, *check));
		error.append("' invalid position");
		if (*check == '{' && ((*token != "{" || &(*token) != &(line->back())) || &(line->back()) == &(line->front())))
			throw InvalidConfigException(error);
		else if (*token == "{")
			this->check_open_curly_bracket(line);
		if (*check == '}' && (&(line->back()) != &(line->front()) || *token != "}"))
			throw InvalidConfigException(error);
		if (*check == ';' && (&(*token) != &(line->back()) || *(token->end() - 1) != ';' || token->find_first_of(";") != token->length() - 1))
			throw InvalidConfigException(error);
	}

	void	ErrorChecker::check_open_curly_bracket(configIterType& line)
	{
		const char*		key_checks[] = {"location"};
		stringCheckType	checks(key_checks, key_checks + 1);
		bool			is_valid = false;

		for (stringCheckIterType check = checks.begin(); check != checks.end(); check++)
		{
			if ((*line).front() == *check)
			{
				is_valid = true;
				break;
			}
		}
		if (is_valid == false)
			throw InvalidConfigException("Error : '" + (*line).front() + "' is an invalid key for start of block");
	}

	void	ErrorChecker::check_location_key(configIterType& line)
	{
		const char*		key_checks[] = {"root", "index", "autoindex", "return", "client_max_body_size"};
		stringCheckType	checks(key_checks, key_checks + 5);
		bool			is_valid = false;

		for (stringCheckIterType check = checks.begin(); check != checks.end(); check++)
		{
			if ((*line).front() == *check)
			{
				is_valid = true;
				break;
			}
		}
		if (is_valid == false)
			throw InvalidConfigException("Error : '" + (*line).front() + "' is an invalid key for location block");
	}
}