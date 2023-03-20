#include "Request.hpp"

namespace	ft
{
	Request::Request(int fd)
	{
		this->_client_fd = fd;
		this->_has_read = false;
	}

	Request::~Request(void) {}

	void	Request::read_buffer(void)
	{
		char*	buffer;

		buffer = static_cast<char*>(malloc(30000 * sizeof(char)));
		recv(this->_client_fd, buffer, 30000, 0);
		std::cout << "buffer: [" << buffer << "]" << std::endl;
		this->_content.append(string(buffer));
		free(buffer);
		// std::cout << "READING..." << std::endl;
		// std::cout << "============================" << std::endl;
		// std::cout << "Content: [" << this->_content << "]" << std::endl;
		// std::cout << "============================" << std::endl;
		if (this->_content.find("\r\n\r\n") != string::npos)
		{
			// std::cout << "Content: [" << this->_content << "]" << std::endl;
			this->parse_buffer();
		}
	}

	void	Request::parse_buffer(void)
	{
		istringstream	buffer_stream(this->_content);
		string			line;

		getline(buffer_stream, line);
		this->_headers.insert(make_pair("method", line.substr(0, line.find_first_of(" "))));
		line = line.substr(line.find_first_of(" ") + 1, line.length());
		this->_headers.insert(make_pair("path", line.substr(0, line.find_first_of(" "))));

		getline(buffer_stream, line);
		this->_headers.insert(make_pair("host", line.substr(line.find_first_of(" ") + 1, line.length())));
		this->_has_read = true;
	}

	void	Request::clear_buffer(void)
	{
		this->_has_read = false;
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

	bool	Request::has_read(void) const
	{
		return (this->_has_read);
	}
}