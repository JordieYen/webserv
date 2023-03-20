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

	bool	Response::path_is_valid_file(string path)
	{
		struct stat	path_stat;
		int			path_exists;

		path = path.substr(path.find("/") == 0);
		path_exists = stat(path.c_str(), &path_stat);
		return (path_exists == 0 && (path_stat.st_mode & S_IFREG));
	}

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
		string											closest_match = "/";

		while (path_to_check != location_map.rend())
		{
			if (request_path.find(path_to_check->first) == 0)
			{
				closest_match = path_to_check->first;
				break;
			}
			path_to_check++;
		}

		if (closest_match.empty())
			std::cout << "path not found" << std::endl;

		return (closest_match);
	}

	string	Response::get_path_to(string directive, string match)
	{
		try
		{
			return (this->_config.get_location_directive(match, directive).front());
		}
		catch (const std::out_of_range& e)
		{
			return (this->_config.get_normal_directive(directive).front());
		}
	}

	string	Response::get_path_to_file(void)
	{
		string	request_path = this->_request.get_header("path");
		string	closest_match = this->get_closest_match();
		string	path_to_file;

		this->_status_code = 200;
		if (this->path_is_valid_file(request_path))
			path_to_file = request_path.substr(1);
		else
		{
			path_to_file = this->get_path_to("root", closest_match) + "/";
			if (closest_match == request_path)
				path_to_file.append(this->get_path_to("index", closest_match));
			else
				path_to_file.append(request_path.substr(closest_match.length() + (request_path[closest_match.length()] == '/')));
			if (!this->path_is_valid_file(path_to_file))
			{
				path_to_file = "public/404.html";
				this->_status_code = 404;
			}
		}
		std::cout << "path_to_file : " << path_to_file << std::endl;
		return (path_to_file);
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
		}
		else
			std::cout << "SOMETHING REALLY BAD HAPPEND!!!" << std::endl;
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
		this->read_config(this->get_path_to_file());
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