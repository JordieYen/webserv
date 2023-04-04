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
		{
			this->_received = true;
			std::cout << "Error : recv returns error..." << std::endl;
		}
		else if (strcmp(buffer, "\r\n") != 0 || !this->_content.empty())
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
			if (line[line.length() - 1] == '\r')
				line = line.substr(0, line.length() - 1);
			if (line.find("Referer") != string::npos)
				this->_referrer = line.substr(line.find(':') + 1);
			if (line.find("Cookie: ") != string::npos)
				this->parse_cookies(line.substr(line.find("Cookie: ") + 8));
			else
			{
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
		}
	}

	void	Request::parse_cookies(string line)
	{
		stringstream	full_line(line);
		string			check;

		while (getline(full_line, check, ' '))
		{
			if (check[check.length() - 1] == ';')
				check = check.substr(0, check.length() - 1);
			if (check.find("=") != string::npos)
				this->_cookies.insert(make_pair(check.substr(0, check.find("=")), check.substr(check.find("=") + 1)));
		}
	}

	bool	Request::context_equals(string context, string value)
	{
		try
		{
			this->_content_context.at(context);
			return (this->_content_context.at(context) == value);
		}
		catch (const std::out_of_range& e) {}
		return (false);
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
		catch (const std::out_of_range& e) {}
		return (false);
	}

	void	Request::parse_chunked_encoded(void)
	{
		string	chunked_encoded_format;
		size_t	chunked_size;
		int		content_length = 0;

		this->_content_body = this->_content;
		this->_content.clear();
		while (this->_content_body.size() > 0)
		{
			if (this->_content_body.find("\r\n") != string::npos)
			{
				chunked_encoded_format = this->_content_body.substr(0, this->_content_body.find("\r\n"));
				chunked_size = 0;
				std::istringstream(chunked_encoded_format) >> std::hex >> chunked_size;
				content_length += chunked_size;
				this->_content_body = this->_content_body.substr(this->_content_body.find("\r\n") + 2);
			}
			this->_content.append(this->_content_body.substr(0, chunked_size));
			this->_content_body = this->_content_body.substr(chunked_size + 2);
		}
		stringstream str_content_length;
		str_content_length << content_length;
		this->_content_context.insert(make_pair(string("Content-Length:"), str_content_length.str()));
		this->_content_body.clear();
	}

	void	Request::parse_urlencoded_body(void)
	{
		stringstream	content_length_stream(this->_content_context.at("Content-Length:"));
		int				content_length;

		content_length_stream >> content_length;
		stringstream	body_stream(this->_content.substr(0, content_length));
		string			pair;

		this->_content_body = this->_content;
		while (getline(body_stream, pair, '&'))
		{
			if (pair.find('=') != string::npos)
				this->_body.insert(make_pair(pair.substr(0, pair.find('=')), pair.substr(pair.find('=') + 1, pair.length())));
			else
				this->_body.insert(make_pair(pair, ""));
		}
		this->_received = true;
	}

	void	Request::parse_multipart_body(void)
	{
		string	part;
		string	first_line;
		string	name;

		this->_content_body = this->_content;
		while (this->_content != "\r\n")
		{
			this->_content = this->_content.substr(this->_content.find("--" + this->_content_context.at("Form-Boundary:")) + this->_content_context.at("Form-Boundary:").length() + 4);
			if (this->_content != "\r\n")
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
					this->_body.insert(make_pair(name, part.substr(part.find("\r\n\r\n") + 4, part.length() - part.find("\r\n\r\n") - 6)));
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
			if (this->_method == "POST")
				this->_content = this->_content.substr(this->_content.find("\r\n\r\n") + 4, this->_content.length());
			else
				this->_received = true;
		}
		if (this->context_equals("Transfer-Encoding:", "chunked") && this->_content.find("0\r\n\r\n") != string::npos)
			this->parse_chunked_encoded();
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
		this->_content_body.clear();
		this->_method.clear();
		this->_path.clear();
		this->_referrer.clear();
		this->_body.clear();
		this->_files.clear();
		this->_cookies.clear();
	}

	int	Request::get_client_fd(void) const
	{
		return (this->_client_fd);
	}

	string	Request::get_header(string key) const
	{
		if (key == "path")
			return (this->_path);
		if (key == "method")
			return (this->_method);
		if (key == "referrer")
			return (this->_referrer);
		return ("Not Found");
	}

	string	Request::get_content_context(string key) const
	{
		try
		{
			if (key == "Content-Type")
				return (this->_content_context.at("Content-Type:"));
			if (key == "Content-Length")
				return (this->_content_context.at("Content-Length:"));
			if (key == "Form-Boundary")
				return (this->_content_context.at("Form-Boundary:"));
		}
		catch (const std::out_of_range& e) {}
		return (string());
	}

	const string&	Request::get_content_body(void) const
	{
		return (this->_content_body);
	}

	const map<string, string>&	Request::get_body_map(void) const
	{
		return (this->_body);
	}

	const map<string, string>&	Request::get_files_map(void) const
	{
		return (this->_files);
	}

	const map<string, string>&	Request::get_cookies_map(void) const
	{
		return (this->_cookies);
	}

	bool	Request::received(void) const
	{
		return (this->_received);
	}
}
