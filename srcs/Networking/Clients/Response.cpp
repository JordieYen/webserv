#include "Response.hpp"

namespace ft
{
	Response::Response(ServerConfig& config, Request* request) : _config(config)
	{
		this->_request = request;
		this->_is_autoindex = false;
		this->_sent = false;
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
		string											request_path = this->_request->get_header("path");
		ServerConfig::locationMapType					location_map = this->_config.get_location_map();
		ServerConfig::locationMapType::reverse_iterator	path_to_check = location_map.rbegin();
		string											closest_match = "/";

		while (path_to_check != location_map.rend() && closest_match.compare("/") == 0)
		{
			if (request_path.find(path_to_check->first) == 0)
			{
				if (path_to_check->second.get_match_type() == LocationBlock::PREFIX || request_path.length() == path_to_check->first.length())
					closest_match = path_to_check->first;
			}
			path_to_check++;
		}
		if (closest_match.empty())
			std::cout << "SOMETHING REALLY BAD HAPPEND!!!" << std::endl;
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

	string	Response::get_path_to_index(string root, string match)
	{
		try
		{
			if (this->_config.get_location_directive(match, "autoindex").front() == "on")
			{
				this->_is_autoindex = true;
				return ("public/autoindex.html");
			}
		}
		catch (const std::out_of_range& e) {}
		return (root.append(this->get_path_to("index", match)));
	}

	string	Response::get_path_to_file(void)
	{
		string	request_path = this->_request->get_header("path");
		string	closest_match = this->get_closest_match();
		string	path_to_file;

		this->_status_code = 200;
		if (this->path_is_valid_file(request_path))
			path_to_file = request_path.substr(1);
		else
		{
			this->_root = this->get_path_to("root", closest_match);
			path_to_file = this->_root + "/";
			if (closest_match == request_path)
				path_to_file = this->get_path_to_index(path_to_file, closest_match);
			else
				path_to_file.append(request_path.substr(closest_match.length() + (request_path[closest_match.length()] == '/')));
			if (!this->path_is_valid_file(path_to_file))
			{
				path_to_file = "public/404.html";
				this->_status_code = 404;
			}
		}
		return (path_to_file);
	}

	void	Response::append_icons(void)
	{
		string			dir_path = this->_root;
		DIR*			dir;
		struct dirent*	dir_entries;

		this->_content.append("\t\t\t<div class=\"table\">");
		dir = opendir(dir_path.c_str());
		while ((dir_entries = readdir(dir)) != NULL)
		{
			string	dir_name = dir_entries->d_name;
			string	full_path = this->_request->get_header("path");

			if (full_path[full_path.length() - 1] != '/')
				full_path.push_back('/');
			full_path += dir_name;

			if (dir_name.find(".") != 0)
			{
				this->_content.append("\t\t\t\t<div class=\"icon\">\n");
				if (dir_entries->d_type == DT_DIR)
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"/public/folder_icons/folder_256.png\" height=\"63\" width=\"63\"></a>\n");
				else if (dir_entries->d_type == DT_REG && (full_path.find(".jpg") != string::npos || full_path.find(".png") != string::npos))
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"" + full_path + "\" height=\"63\" width=\"63\"></a>\n");
				else
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"/public/folder_icons/folder_256.png\" height=\"63\" width=\"63\"></a>\n");
				this->_content.append("\t\t\t\t\t<p class=\"icon_name\">" + dir_name + "</p>\n");
				this->_content.append("\t\t\t\t</div>\n");
			}
		}
		closedir(dir);
	}

	void	Response::handle_autoindex(string line)
	{
		if (line.compare("\t\t\t\t<p class=\"header_title\"></p>") == 0)
		{
			this->_content.append(line.substr(0, line.length() - 4) + this->_root + "</p>");
		}
		else if (line.compare("\t\t\t<div class=\"table\">") == 0)
			this->append_icons();
		else
			this->_content.append(line + "\n");
	}

	void	Response::read_config(string file_name)
	{
		string		line;
		ifstream	config_file(file_name.c_str());

		if (config_file.is_open())
		{
			while (getline(config_file, line))
			{
				if (this->_is_autoindex)
					this->handle_autoindex(line);
				else
					this->_content.append(line + "\n");
			}
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
		header.append(this->_content);
		this->_content.clear();
		this->_content = header;
	}

	void	Response::send_to_client(void)
	{
		int	length_to_send = this->_content.length();
		if (length_to_send <= 100000)
			this->_sent = true;
		else
			length_to_send = 100000;
		send(this->_request->get_client_fd(), this->_content.substr(0, length_to_send).c_str(), length_to_send, 0);
		string trimmed_content = this->_content.substr(length_to_send);
		this->_content.clear();
		this->_content = trimmed_content;
	}

	void	Response::handle_get(void)
	{
		if (this->_content.empty())
		{
			this->read_config(this->get_path_to_file());
			this->prepend_header();
		}
		this->send_to_client();
	}

	void	Response::handle_post(void)
	{
		
	}

	void	Response::handle_delete(void)
	{
		
	}

	void	Response::handle_bad_request(void)
	{
		if (this->_content.empty())
		{
			this->_status_code = 400;
			this->read_config("public/404.html");
			this->prepend_header();
		}
		this->send_to_client();
	}

	void	Response::handle_methods(void)
	{
		if (this->_request->get_header("method") == "GET")
			this->handle_get();
		else if (this->_request->get_header("method") == "POST")
			this->handle_post();
		else if (this->_request->get_header("method") == "DELETE")
			this->handle_delete();
		else if (this->_request->get_header("method") == "BAD REQUEST")
			this->handle_bad_request();
	}

	bool	Response::sent(void)
	{
		if (this->_sent)
		{
			this->_root.clear();
			this->_is_autoindex = false;

			this->_sent = false;
			this->_content.clear();
			this->_request->clear_buffer();
			return (true);
		}
		return (false);
	}
}
