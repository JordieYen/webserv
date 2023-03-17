#include "Response.hpp"

namespace ft
{
	Response::Response(ServerConfig& config, Request& request) : _config(config), _request(request)
	{
		std::cout << "method: " << request.get_header("method") << std::endl;
		std::cout << "path: " << request.get_header("path") << std::endl;
		std::cout << "host: " << request.get_header("host") << std::endl;
	}

	Response::~Response(void) {}

	string	Response::get_content_length(void)
	{
		stringstream	length_stream;

		length_stream << this->_content.length();
		return (length_stream.str());
	}

	string	Response::get_closest_match(void)
	{
		string											request_path = this->_request.get_header("path");
		ServerConfig::locationMapType					location_map = this->_config.get_location_map();
		ServerConfig::locationMapType::reverse_iterator	path_to_check = location_map.rbegin();
		string											closest_match;

		while (request_path.find(path_to_check->first) != 0 && path_to_check != location_map.rend())
		{
			std::cout << "path_to_check: " << path_to_check->first << std::endl;
			std::cout << "request_path: " << request_path << std::endl;
			path_to_check++;
		}
		if (request_path.find(path_to_check->first) == 0)
			closest_match = path_to_check->first;
		std::cout << "closest match = " << closest_match << std::endl;

		if (closest_match.empty())
			std::cout << "path not found" << std::endl;

		return (closest_match);
	}

	string	Response::get_path_to_index(void)
	{
		string	closest_match = this->get_closest_match();
		string	path_to_index;

		try
		{
			std::cout << closest_match << std::endl;
			path_to_index = this->_config.get_location_directive(closest_match, "root").front() + "/";
		}
		catch (const std::out_of_range& e)
		{
			path_to_index = this->_config.get_normal_directive("root").front() + "/";
		}
		try
		{
			path_to_index.append(this->_config.get_location_directive(closest_match, "index").front());
		}
		catch (const std::out_of_range& e)
		{
			path_to_index.append(this->_config.get_normal_directive("index").front());
		}
		std::cout << path_to_index << std::endl;
		return (path_to_index);
	}

	void	Response::read_config(string file_name)
	{
		string		line;
		ifstream	config_file(file_name.c_str());

		if (config_file.is_open())
		{
			while (getline(config_file, line))
				this->_content = this->_content.append(line) + "\n";
			config_file.close();
			this->_status_code = 200;
		}
		else
		{
			this->_status_code = 404;
			std::cout << "404 not found : '" << file_name <<"' not found" << std::endl;
		}
	}

	void	Response::prepend_header(void)
	{
		string	header = "HTTP/1.1 ";

		switch (this->_status_code)
		{
			case 200:	header.append("200 OK\r\n"); break;
			case 400:	header.append("400 Bad Request\r\n"); break;
			case 403:	header.append("403 Forbidden\r\n"); break;
			case 404:	header.append("404 Not Found\r\n"); break;
		}
		header.append("Content-Type: */*\r\n");
		header.append("Content-Length: ");
		header.append(this->get_content_length());
		header.append("\r\n\r\n");
		this->_content = header.append(this->_content);
	}

	void	Response::handle_get(void)
	{
		this->read_config(this->get_path_to_index());
		this->prepend_header();
		std::cout << "==========index=========" << std::endl;
		std::cout << this->_content << std::endl;
		std::cout << "==========index=========" << std::endl;

		send(this->_request.get_client_fd(), this->_content.c_str(), this->_content.length(), 0);
	}

	void	Response::handle_methods(void)
	{
		if (this->_request.get_header("method") == "GET")
			handle_get();
		// else if (request.get_header("method") == "POST")
		// 	handle_post();
		// else if (request.get_header("method") == "DELETE")
		// 	handle_delete();
	}
}