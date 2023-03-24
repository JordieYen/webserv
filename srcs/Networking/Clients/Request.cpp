#include "Request.hpp"

namespace	ft
{
	Request::Request(int fd)
	{
		this->_client_fd = fd;
		this->_received = false;
	}

	Request::~Request(void) {}

	void	Request::validate_content(void)
	{
		if (this->_content.find("GET") == std::string::npos && this->_content.find("POST") == std::string::npos && this->_content.find("DELETE") == std::string::npos)
		{
			std::cout << "invalid request!" << std::endl;
			this->_content.clear();
			this->_received = true;
			this->_content.append("BAD REQUEST");
		}
		// else if ()
	}

	void	Request::read_buffer(void)
	{
		char*	buffer;

		buffer = static_cast<char*>(calloc(30000 * sizeof(char), sizeof(char)));
		recv(this->_client_fd, buffer, 30000, 0);
		std::cout << "buffer: [" << buffer << "]" << std::endl;
		this->_content.append(string(buffer));
		free(buffer);
		if (this->_content == "\r\n")
			this->_content.clear();
		// std::cout << "READING..." << std::endl;
		// std::cout << "============================" << std::endl;
		// std::cout << "Content: [" << this->_content << "]" << std::endl;
		// std::cout << "============================" << std::endl;
		if (this->_content.find("\r\n\r\n") != string::npos)
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

		if (this->_content == "BAD REQUEST")
		{
			this->_headers.insert(make_pair("method", this->_content));
			return;
		}
		getline(buffer_stream, line);
		this->_headers.insert(make_pair("method", line.substr(0, line.find_first_of(" "))));
		line = line.substr(line.find_first_of(" ") + 1, line.length());
		path = line.substr(0, line.find_first_of(" "));
		if (path[0] != '/' && path[path.length() - 1] == '/')
			path = path.substr(0, path.length() - 1);
		this->_headers.insert(make_pair("path", path));

		getline(buffer_stream, line);
		this->_headers.insert(make_pair("host", line.substr(line.find_first_of(" ") + 1, line.length())));
		this->_received = true;
	}

	void	Request::clear_buffer(void)
	{
		this->_received = false;
		this->_content.clear();
		this->_headers.clear();
	}

	int	Request::get_client_fd(void) const
	{
		return (this->_client_fd);
	}

	string	Request::get_header(string key) const
	{
		return (this->_headers.at(key));
	}

	bool	Request::received(void) const
	{
		return (this->_received);
	}
}
