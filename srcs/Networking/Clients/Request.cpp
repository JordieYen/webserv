#include "Request.hpp"

namespace	ft
{
	Request::Request(int fd)
	{
		this->_client_fd = fd;
		this->_received = false;
	}

	Request::~Request(void) {}

	void	Request::read_from_client(void)
	{
		int		read_bytes;
		char*	buffer = static_cast<char*>(calloc(65536 * sizeof(char), sizeof(char)));

		if ((read_bytes = recv(this->_client_fd, buffer, 65536, 0)) < 0)
			std::cout << "Error : recv returns error..." << std::endl;
		if (strcmp(buffer, "\r\n") != 0 || !this->_content.empty())
			this->_content.append(buffer, read_bytes);
		free(buffer);
	}

	bool	Request::header_not_valid(void)
	{
		string			first_line = this->_content.substr(0, this->_content.find("\r\n"));
		stringstream	full_first_line(first_line.c_str());
		string			word;

		for (size_t i = 1; i <= 4; i++)
		{
			if (!(getline(full_first_line, word, ' ')) && i != 4)
				return (true);
			else
			{
				if (i == 2 && (word[0] != '/'))
					return (true);
				else if (i == 3 && (word.find("HTTP/1.1") != 0))
					return (true);
			}
		}
		return (false);
	}

	void	Request::set_bad_request(void)
	{
		std::cout << "invalid request!" << std::endl;
		this->_content.clear();
		this->_received = true;
	}

	void	Request::parse_header(void)
	{
		stringstream	full_header(this->_content.substr(0, this->_content.find("\r\n\r\n")));
		string			line;

		getline(full_header, line);
		stringstream	full_first_line(line);

		full_first_line >> this->_method;
		full_first_line >> this->_path;
		if (this->_path[0] != '/' && this->_path[this->_path.length() - 1] == '/')
			this->_path = this->_path.substr(0, this->_path.length() - 1);

		const char*			content_context_checks[] = {"Content-Type", "Content-Length", "Transfer-Encoding"};
		vector<string>		content_context(content_context_checks, content_context_checks + 3);

		while (getline(full_header, line))
		{
			line = line.substr(0, line.length());
			if (line.find("Referer") != string::npos)
				this->_referrer = line.substr(line.find(':') + 1);
			if (line.find("Cookie: ") != string::npos)
			{
				// std::cout << line << std::endl;
				this->_cookies.insert(make_pair(line.substr((line.find("Cookie: ") + 8), line.find(";") - (line.find("Cookie: ") + 8)), line.substr((line.find(";") + 2))));
			}
			for (vector<string>::iterator context = content_context.begin(); context != content_context.end(); context++)
			{
				if (line.find(*context) != string::npos)
				{
					if (line.find("multipart/form-data") != string::npos)
					{
						this->_content_context.insert(make_pair("Form-Boundary:", line.substr(line.find('=') + 1, line.length())));
						line = line.substr(0, line.find(';'));
					}
					this->_content_context.insert(make_pair(line.substr(0, line.find(' ')), line.substr(line.find(' ') + 1, line.length())));
				}
			}
		}

		// for (map<string, string>::iterator cookie = this->_cookies.begin(); cookie != this->_cookies.end(); cookie++)
		// {
		// 	std::cout << "cookie [" << cookie->first << "][" << cookie->second << "]" << std::endl;
		// }
		std::cout << this->_content << std::endl;
	}

	bool	Request::context_equals(string context, string value)
	{
		try
		{
			this->_content_context.at(context);
			return (this->_content_context.at(context).compare(value) == 0);
		}
		catch (const std::out_of_range& e)
		{
			return (false);
		}
	}

	bool	Request::body_has_minimum_length(void)
	{
		try
		{
			stringstream	content_length_stream(this->_content_context.at("Content-Length:"));
			size_t			content_length;

			content_length_stream >> content_length;
			return (this->_content.length() >= content_length);
		}
		catch (const std::out_of_range& e)
		{
			this->_content_context.insert(make_pair("Content-Length:", "0"));
			return (true);
		}
	}

	void	Request::parse_urlencoded_body(void)
	{
		stringstream	content_length_stream(this->_content_context.at("Content-Length:"));
		int				content_length;

		content_length_stream >> content_length;
		stringstream	body_stream(this->_content.substr(0, content_length));
		string			pair;

		while (getline(body_stream, pair, '&'))
		{
			if (pair.find('=') != string::npos)
				this->_body.insert(make_pair(pair.substr(0, pair.find('=')), pair.substr(pair.find('=') + 1, pair.length())));
			else
				this->_body.insert(make_pair(pair, ""));
		}
		// for (std::map<string, string>::iterator it = this->_body.begin(); it != this->_body.end(); it++)
		// {
		// 	std::cout << "[" << it->first << "]=[" << it->second << "]" << std::endl;
		// }
		this->_received = true;
	}

	void	Request::parse_multipart_body(void)
	{
		string	part;
		string	first_line;
		string	name;

		while (this->_content.compare("\r\n") != 0)
		{
			this->_content = this->_content.substr(this->_content.find("--" + this->_content_context.at("Form-Boundary:")) + this->_content_context.at("Form-Boundary:").length() + 4);
			if (this->_content.compare("\r\n") != 0)
			{
				part = this->_content.substr(0, this->_content.find("--" + this->_content_context.at("Form-Boundary:")));
				first_line = part.substr(0, part.find("\r\n"));
				if (first_line.find("filename") != string::npos)
				{
					name = first_line.substr(first_line.find_last_of('=') + 2, first_line.length() - first_line.find_last_of('=') - 3);
					this->_files.insert(make_pair(name, part.substr(part.find("\r\n\r\n") + 4)));
				}
				else
				{
					name = first_line.substr(first_line.find('=') + 2, first_line.length() - first_line.find('=') - 3);
					this->_body.insert(make_pair(name, part.substr(part.find("\r\n\r\n") + 4)));
				}
				name.clear();
			}
		}
		this->_received = true;
	}

	void	Request::handle_request(void)
	{
		this->read_from_client();
		if (this->_method.empty() && this->_path.empty() &&
			this->_content.find("\r\n\r\n") != string::npos)
		{
			if (this->header_not_valid())
				this->set_bad_request();
			else
				this->parse_header();
			if (this->_method.compare("POST") == 0)
				this->_content = this->_content.substr(this->_content.find("\r\n\r\n") + 4, this->_content.length());
			else
				this->_received = true;
		}
		if (this->context_equals("Content-Type:", "application/x-www-form-urlencoded") &&
			this->body_has_minimum_length())
			this->parse_urlencoded_body();
		else if (this->context_equals("Content-Type:", "multipart/form-data") &&
			this->_content.find("--" + this->_content_context.at("Form-Boundary:") + "--") != string::npos)
			this->parse_multipart_body();
	}

	void	Request::clear(void)
	{
		this->_received = false;
		this->_content.clear();
		this->_content_context.clear();
		this->_method.clear();
		this->_path.clear();
		this->_body.clear();
	}

	int	Request::get_client_fd(void) const
	{
		return (this->_client_fd);
	}

	string	Request::get_header(string key) const
	{
		if (key == "path")
			return (this->_path);
		else if (key == "method")
			return (this->_method);
		else if (key == "referrer")
			return (this->_referrer);
		else
			return ("Not Found");
	}

	map<string, string>	Request::get_body_map() const
	{
		return (this->_body);
	}

	map<string, string>	Request::get_files_map() const
	{
		return (this->_files);
	}

	bool	Request::received(void) const
	{
		return (this->_received);
	}
}
