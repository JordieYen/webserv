#include "Response.hpp"

namespace ft
{
	Response::Response(SimpleServer& server, Request* request) : _server(server), _config(server.get_config())
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

	string	Response::get_string_from_numeral(int	input)
	{
		stringstream	length_stream;

		length_stream << input;
		return (length_stream.str());
	}

	string	Response::get_status_message()
	{
		switch (this->_status_code)
		{
			case 200:	return (string("200 OK"));
			case 204:	return (string("204 No Content"));
			case 301:	return (string("301 Moved Permanently"));
			case 303:	return (string("303 See Other"));
			case 400:	return (string("400 Bad Request"));
			case 404:	return (string("404 Not Found"));
			case 405:	return (string("405 Method Not Allowed"));
			case 413:	return (string("413 Payload Too Large"));
		}
		return (string("SOMETHING REALLY BAD HAPPENED!!!"));
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

	string	Response::get_path_to(string directive)
	{
		try
		{
			return (this->_config.get_location_directive(this->_closest_match, directive).front());
		}
		catch (const std::out_of_range& e)
		{
			return (this->_config.get_normal_directive(directive).front());
		}
	}

	string	Response::get_path_to_error(void)
	{
		try
		{
			string path = this->_root + "/" + this->_config.get_normal_directive(this->get_string_from_numeral(this->_status_code)).front();

			if (this->path_is_valid_file(path))
				return (path);
		}
		catch (const std::out_of_range& e) {}

		return (string("public/error.html"));
	}

	string	Response::get_path_to_index(void)
	{
		try
		{
			if (this->_config.get_location_directive(this->_closest_match, "autoindex").front() == "on")
			{
				this->_is_autoindex = true;
				return ("public/autoindex.html");
			}
		}
		catch (const std::out_of_range& e) {}
		return (this->_root + "/" + this->get_path_to("index"));
	}

	string	Response::get_path_to_file(void)
	{
		string	request_path = this->_request->get_header("path");
		string	path_to_file;

		if (this->path_is_valid_file(request_path))
			path_to_file = request_path.substr(1);
		else
		{
			path_to_file = this->_root + "/";
			if (this->_closest_match == request_path)
				path_to_file = this->get_path_to_index();
			else
				path_to_file.append(request_path.substr(this->_closest_match.length() + (request_path[this->_closest_match.length()] == '/')));
			if (!this->path_is_valid_file(path_to_file))
			{
				this->_status_code = 404;
				path_to_file = this->get_path_to_error();
			}
		}
		return (path_to_file);
	}

	string	Response::generate_random_hash(void)
	{
		std::random_device	rd;
		std::mt19937		rng(rd());
		std::string			alnum = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string			hash;

		for (int i = 0; i < 32; i++)
			hash.push_back(alnum[std::uniform_int_distribution<int>(0, 61)(rng)]);
		return (hash);
	}

	string	Response::generate_time_limit(int minutes)
	{
		time_t	t = time(NULL) + (minutes * 60);
		struct tm* gmt_t = gmtime(&t);

		char	timestamp[30];

		strftime(timestamp, sizeof(timestamp), "%a, %d-%b-%Y %H:%M:%S GMT", gmt_t);
		return (timestamp);
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
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"/public/icons/folder_256.png\" height=\"63\" width=\"63\"></a>\n");
				else if (dir_entries->d_type == DT_REG && (full_path.find(".jpg") != string::npos || full_path.find(".png") != string::npos))
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"" + full_path + "\" height=\"63\" width=\"63\"></a>\n");
				else
					this->_content.append("\t\t\t\t\t<a href=\"" + full_path + "\"><img src=\"/public/icons/folder_256.png\" height=\"63\" width=\"63\"></a>\n");
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

	void	Response::read_file(string file_name)
	{
		string		line;
		ifstream	file(file_name.c_str());

		if (file.is_open())
		{
			while (getline(file, line))
			{
				if (this->_is_autoindex)
					this->handle_autoindex(line);
				else if (this->_status_code != 200 && line.compare("\t<body>") == 0 && file_name == "public/error.html")
					this->_content.append("\t\t<h1>" + this->get_status_message() + "</h1>\n");
				else
					this->_content.append(line + "\n");
			}
			file.close();
		}
		else
			std::cout << "SOMETHING REALLY BAD HAPPEND!!!" << std::endl;
	}

	bool	Response::handle_return(void)
	{
		this->_status_code = 301;
		this->_content.append("HTTP/1.1 " + this->get_status_message() + "\r\n");
		try
		{
			this->_content.append("Location: " + this->_config.get_location_directive(this->_closest_match, "return").front() + "\r\n\r\n");
			return (true);
		}
		catch (const std::out_of_range& e) {}
		try
		{
			this->_content.append("Location: " + this->_config.get_normal_directive("return").front() + "\r\n\r\n");
			return (true);
		}
		catch (const std::out_of_range& e) {}
		this->_status_code = 200;
		this->_content.clear();
		return (false);
	}

	void	Response::prepend_header(void)
	{
		string	header = "HTTP/1.1 ";

		header.append(this->get_status_message() + "\r\n");
		if (this->_request->get_header("path").find(".ico") != string::npos)
			header.append("Content-Type: image/x-icon\r\n");
		else
			header.append("Content-Type: */*; charset=utf-8\r\n");
		header.append("Content-Length: ");
		header.append(this->get_string_from_numeral(this->_content.length()));
		header.append("\r\n\r\n");
		header.append(this->_content);
		this->_content.clear();
		this->_content = header;
	}

	void	Response::send_to_client(void)
	{
		int	length_to_send = this->_content.length();
		if (length_to_send <= 50000)
			this->_sent = true;
		else
			length_to_send = 50000;
		if (send(this->_request->get_client_fd(), this->_content.substr(0, length_to_send).c_str(), length_to_send, 0) == -1)
			std::cout << "Error : send returns error..." << std::endl;
		this->_content = this->_content.substr(length_to_send);
	}

	bool	Response::check_error(void)
	{
		string			request_method = this->_request->get_header("method");
		vector<string>	allowed_methods;

		if (request_method.empty())
			this->_status_code = 400;
		else
		{
			if (request_method.compare("POST") == 0)
			{
				string	max_size_string;
				try
				{
					max_size_string = this->_config.get_location_directive(this->_closest_match, "client_max_body_size").front();
				}
				catch (const std::out_of_range& e)
				{
					max_size_string = this->_config.get_normal_directive("client_max_body_size").front();
				}
				map<string, string>	files = this->_request->get_files_map();
				stringstream		max_size_stream(max_size_string);
				size_t				max_size;

				max_size_stream >> max_size;
				for (map<string, string>::iterator file = files.begin(); file != files.end(); file++)
				{
					if (file->second.length() > max_size * 1000000)
					{
						this->_status_code = 413;
						return (true);
					}
				}
			}
			try
			{
				allowed_methods = this->_config.get_location_directive(this->_closest_match, "allowed_methods");
			}
			catch (const std::out_of_range& e)
			{
				allowed_methods = this->_config.get_normal_directive("allowed_methods");
			}
			for (vector<string>::iterator method = allowed_methods.begin(); method != allowed_methods.end(); method++)
			{
				if (method->compare(request_method) == 0)
					return (false);
			}
			this->_status_code = 405;
		}
		return (true);
	}

	void	Response::handle_error(void)
	{
		if (this->_content.empty())
		{
			this->read_file(this->get_path_to_error());
			this->prepend_header();
		}
		this->send_to_client();
	}

	bool	Response::check_cgi(void)
	{
		try
		{
			return (!this->_config.get_location_directive(this->_closest_match, "cgi_pass").empty());
		}
		catch (const std::out_of_range& e) {}
		return (false);
	}

	void	Response::handle_cgi(void)
	{
		if (this->_content.empty())
		{
			int		fds[2];
			pid_t	pid;
			string	path = this->_root + "/" + this->get_path_to("index");

			pipe(fds);
			pid = fork();
			if (pid == 0)
			{
				vector<char*>	args;

				args.push_back(strdup("/usr/bin/python3"));
				args.push_back(strdup(path.c_str()));
				args.push_back(nullptr);

				vector<char*>	envp;

				envp.push_back(strdup(string("REQUEST_METHOD=" + this->_request->get_header("method")).c_str()));
				envp.push_back(strdup(string("PATH_INFO=" + this->_request->get_header("path")).c_str()));
				envp.push_back(strdup(string("USERNAME=" + this->_username).c_str()));
				envp.push_back(nullptr);

				dup2(fds[1], STDOUT_FILENO);
				close(fds[1]);
				close(fds[0]);
				execve("/usr/bin/python3", args.data(), envp.data());
			}
			else if (pid > 0)
			{
				close(fds[1]);
				waitpid(pid, NULL, 0);

				char*	buffer = static_cast<char*>(calloc(65535 * sizeof(char), sizeof(char)));

				read(fds[0], buffer, 65535);
				this->_content.append(buffer);
				free(buffer);
			}
			else
				std::cout << "Error : fork returns error..." << std::endl;
			this->prepend_header();
		}
		this->send_to_client();
	}

	void	Response::handle_get(void)
	{
		if (this->_content.empty())
		{
			if (!this->handle_return())
			{
				this->read_file(this->get_path_to_file());
				this->prepend_header();
			}
		}
		this->send_to_client();
	}

	void	Response::handle_post(void)
	{
		map<string, string>	files = this->_request->get_files_map();

		for (map<string, string>::iterator file = files.begin(); file != files.end(); file++)
		{
			ofstream	new_file((this->_root + "/" + file->first).c_str());

			new_file << file->second;
			new_file.close();
		}
		this->_status_code = 301;
		this->_content.append("HTTP/1.1 " + this->get_status_message() + "\r\n");

		if (this->_request->get_body_map().find("username") != this->_request->get_body_map().end())
		{
			string	hash = this->generate_random_hash();

			this->_server.set_cookie("USR_KYZ", hash, this->_request->get_body_map().at("username"));
			this->_content.append("Location: /homepage\r\n");
			this->_content.append("Set-Cookie: USR_KYZ=" + hash + "; Expires=" + this->generate_time_limit(5) + "\r\n\r\n");
		}
		else
			this->_content.append("Location: " + this->_request->get_header("referrer") + "\r\n");
		this->send_to_client();
	}

	void	Response::handle_delete(void)
	{
		string	request_path = this->_request->get_header("path");

		if (this->_closest_match == request_path)
			this->_status_code = 405;
		else if (remove((this->_root + request_path.substr(this->_closest_match.length())).c_str()) != 0)
			this->_status_code = 204;
		this->_status_code = 301;
		this->_content.append("HTTP/1.1 " + this->get_status_message() + "\r\n");
		this->_content.append("Location: " + this->_request->get_header("referrer") + "\r\n\r\n");
		this->send_to_client();
	}

	void	Response::handle_methods(void)
	{
		this->_closest_match = get_closest_match();
		this->_root = this->get_path_to("root");
		this->_status_code = 200;

		if (this->_request->get_cookies_map().find("USR_KYZ") != this->_request->get_cookies_map().end())
			this->_username = this->_server.get_cookie_value("USR_KYZ", this->_request->get_cookies_map().at("USR_KYZ"));

		if (this->check_error())
			this->handle_error();
		else if (this->check_cgi())
			this->handle_cgi();
		else if (this->_request->get_header("method") == "GET")
			this->handle_get();
		else if (this->_request->get_header("method") == "POST")
			this->handle_post();
		else if (this->_request->get_header("method") == "DELETE")
			this->handle_delete();
	}

	bool	Response::sent(void)
	{
		if (this->_sent)
		{
			this->_root.clear();
			this->_is_autoindex = false;

			this->_sent = false;
			this->_content.clear();
			this->_request->clear();
			return (true);
		}
		return (false);
	}
}
