#include "Request.hpp"

namespace	ft
{
	Request::Request(int fd)
	{
		this->_client_fd = fd;
		this->_received = false;
	}

	Request::~Request(void) {}

	void	Request::set_bad_request(void)
	{
		std::cout << "invalid request!" << std::endl;
		this->_content.clear();
		this->_received = true;
	}

	void	Request::validate_content(void)
	{
		string			first_line = this->_content.substr(0, this->_content.find("\r\n"));
		stringstream	full_request(this->_content.c_str());
		string			line;
		stringstream	full_first_line(first_line.c_str());
		string			word;

		for (size_t i = 1; i <= 4; i++)
		{
			if (!(getline(full_first_line, word, ' ')) && i != 4)
				set_bad_request();
			else
			{
				if (i == 1 && (word != "GET" && word != "POST" && word != "DELETE"))
					return set_bad_request();
				else if (i == 2 && (word[0] != '/'))
					return set_bad_request();
				else if (i == 3 && (word.find("HTTP/1.1") != 0))
					return set_bad_request();
			}
		}

		const char*			content_context_checks[] = {"Content-Type", "Content-Length", "Transfer-Encoding"};
		vector<string>		content_context(content_context_checks, content_context_checks + 3);
		
		while (getline(full_request, line, '\n'))
		{
			for (vector<string>::iterator context = content_context.begin(); context != content_context.end(); context++)
			{
				if (line.find(*context) != string::npos)
					this->_content_context.insert(make_pair(line.substr(0, line.find(' ')), line.substr(line.find(' ') + 1, line.length() - 5)));
			}
		}

		if (!this->_content_context.empty())		
		{
			for (map<string, string>::iterator context = this->_content_context.begin(); context != this->_content_context.end(); context++)
				std::cout << "context : [" << context->first << "] [" << context->second << "]" << std::endl;
			this->_body = this->_content.substr(this->_content.find("\r\n\r\n") + 4, this->_content.length());
			std::cout << "body [" <<  this->_body << "]" << std::endl;
		}
		else
			std::cout << "no context" << std::endl;
	}

	void	Request::read_buffer(void)
	{
		char*	buffer;

		buffer = static_cast<char*>(calloc(30000 * sizeof(char), sizeof(char)));
		recv(this->_client_fd, buffer, 30000, 0);
		std::cout << "buffer: [" << buffer << "]" << std::endl;
		if (this->_content.length() > 0 || strcmp(buffer, "\r\n") != 0)
			this->_content.append(string(buffer));
		free(buffer);
		// std::cout << "READING..." << std::endl;
		// std::cout << "============================" << std::endl;
		// std::cout << "Content: [" << this->_content << "]" << std::endl;
		// std::cout << "============================" << std::endl;
		if (!(this->_content.empty()) && this->_content.find("\r\n\r\n") != string::npos)
		{
			// std::cout << "DONE Content: [" << this->_content << "]" << std::endl;
			this->validate_content();
			this->parse_buffer();
		}
	}

	void	Request::parse_buffer(void)
	{
		istringstream	buffer_stream(this->_content);
		string			line;
		string			path;

		if (this->_content.empty())
			return;
		getline(buffer_stream, line);
		this->_method = line.substr(0, line.find_first_of(" "));
		line = line.substr(line.find_first_of(" ") + 1, line.length());
		path = line.substr(0, line.find_first_of(" "));
		if (path[0] != '/' && path[path.length() - 1] == '/')
			path = path.substr(0, path.length() - 1);
		this->_path = path;
		this->_received = true;
	}

	void	Request::clear_buffer(void)
	{
		this->_received = false;
		this->_content.clear();
		this->_method.clear();
		this->_path.clear();
	}

	int	Request::get_client_fd(void) const
	{
		return (this->_client_fd);
	}

	string	Request::get_header(string key) const
	{
		if (key == "path")
			return(this->_path);
		else if (key == "method")
			return (this->_method);
		else
			return ("Not Found");
	}

	bool	Request::received(void) const
	{
		return (this->_received);
	}
}
