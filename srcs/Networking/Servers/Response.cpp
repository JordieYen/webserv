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

	void	Response::handle_methods(void)
	{
		if (this->_request.get_header("method") == "GET")
			handle_get();
		// else if (request.get_header("method") == "POST")
		// 	handle_post();
		// else if (request.get_header("method") == "DELETE")
		// 	handle_delete();

		send(this->_request.get_client_fd(), "Hello from server!", 19, 0);
	}

	void	Response::handle_get(void)
	{
		string	path_to_index = this->get_path_to_index();

		this->read_config(path_to_index);
		// std::cout << "==========index=========" << std::endl;
		// std::cout << this->_index << std::endl;
		// std::cout << "==========index=========" << std::endl;

		exit(1);
	}

	string	Response::get_path_to_index(void)
	{
		string	closest_match = this->get_closest_match();
		string	path_to_index;

		try
		{
			path_to_index = this->_config.get_location_directive(closest_match, "root").front() + "/";
			path_to_index.append(this->_config.get_location_directive(closest_match, "index").front());
		}
		catch(const std::out_of_range& e)
		{
			path_to_index = this->_config.get_normal_directive("root").front() + "/";
			try {
				path_to_index.append(this->_config.get_location_directive(closest_match, "index").front());
			}
			catch (const std::out_of_range& e)
			{
				path_to_index.append(this->_config.get_normal_directive("index").front());
			}
		}

		return (path_to_index);
	}

	string	Response::get_closest_match(void)
	{
		string							request_path = this->_request.get_header("path");
		ServerConfig::locationMapType	location_map = this->_config.get_location_map();
		int								similarity_score = INT_MIN;
		string							closest_match;

		for (ServerConfig::locationMapType::iterator path = location_map.begin(); path != location_map.end(); path++)
		{
			int	temp_score = path->first.compare(request_path);
			if (temp_score > similarity_score && similarity_score != 0)
			{
				similarity_score = temp_score;
				closest_match = path->first;
			}
		}
		std::cout << "closest match = " << closest_match << std::endl;

		if (similarity_score > 0)
			std::cout << "path not found" << std::endl;

		return (closest_match);
	}

	void	Response::read_config(string file_name)
	{
		string		line;
		ifstream	config_file(file_name.c_str());

		if (config_file.is_open())
		{
			while (getline(config_file, line))
				this->_index = this->_index.append(line) + "\n";
			config_file.close();
		}
		else
			std::cout << "404 not found : '" << file_name <<"' not found" << std::endl;
	}
}