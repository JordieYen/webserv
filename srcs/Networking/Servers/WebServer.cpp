#include "WebServer.hpp"

namespace ft
{
	WebServer::WebServer(void) : _parser(DEFAULT_CONFIG_PATH)
	{
		this->setup();
	}

	WebServer::WebServer(string config_path) : _parser(config_path)
	{
		this->setup();
	}

	WebServer::~WebServer(void)
	{
		for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
		{
			delete server->second;
		}
	}

	void	WebServer::setup(void)
	{
		this->_parser.read_config();
		this->_parser.check_config();
		this->_parser.parse_config();

		vector<ft::ServerConfig>	configs = this->_parser.get_parsed_server_configs();

		for (vector<ft::ServerConfig>::iterator it = configs.begin(); it != configs.end(); it++)
		{
			SimpleServer*	new_server = new SimpleServer(*it);
			pollFdType		new_pollfd;

			new_pollfd.fd = new_server->get_server_fd();
			new_pollfd.events = POLLIN;
			this->_pollfds.push_back(new_pollfd);
			new_server->set_pollfd_index(this->_pollfds.size() - 1);
			this->_servers.insert(make_pair(new_server->get_port(), new_server));
		}
	}

	void	WebServer::launch(void)
	{
		while (true)
		{
			int rc = poll(this->_pollfds.data(), this->_pollfds.size(), 1000);
			if (rc < 0)
				exit(0);
			std::cout << "Poll loop..." << std::endl;
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				if (this->_pollfds.at(server->second->get_pollfd_index()).revents == 0)
					continue;
				std::cout << "Server checking with fd : " << server->second->get_server_fd() << std::endl;
				if (this->_pollfds.at(server->second->get_pollfd_index()).revents & POLLIN)
				{
					int	client_fd = server->second->accept_connection();
					if (client_fd != -1)
					{
						std::cout << "Server pollin..." << std::endl;
						Client*		new_client = new Client(client_fd);
						pollFdType	new_pollfd;

						new_pollfd.fd = client_fd;
						new_pollfd.events = POLLIN;
						this->_pollfds.push_back(new_pollfd);
						// cout << new_client->_pollfd << endl;
						new_client->set_pollfd_index(this->_pollfds.size() - 1);
						this->_clients.insert(make_pair(server->second->get_port(), new_client));
						std::cout << "Client created..." << std::endl;
					}
				}
			}
			for (clientMapType::iterator client = this->_clients.begin();  client != this->_clients.end(); client++)
			{
				if (this->_pollfds.at(client->second->get_pollfd_index()).revents == 0)
					continue;
				cout << "Poll size : " << this->_pollfds.size() << endl;
				std::cout << "Client checking with fd : " << client->second->get_fd() << std::endl;
				printf("%d revents = %s %s %s\n", client->second->get_fd(),
					(this->_pollfds.at(client->second->get_pollfd_index()).revents & POLLIN) ? "POLLIN" : "",
					(this->_pollfds.at(client->second->get_pollfd_index()).revents & POLLOUT) ? "POLLOUT" : "",
					(this->_pollfds.at(client->second->get_pollfd_index()).revents & POLLHUP) ? "POLLHUP" : "");

				if (this->_pollfds[client->second->get_pollfd_index()].revents & POLLIN)
				{
					std::cout << "Client pollin..." << std::endl;
					client->second->read_buffer();
					if (client->second->get_request().has_read())
						this->_pollfds.at(client->second->get_pollfd_index()).events = POLLOUT;
				}
				else if (this->_pollfds[client->second->get_pollfd_index()].revents & POLLOUT)
				{
					std::cout << "Client pollout..." << std::endl;
					std::cout << "method: " << client->second->get_request().get_header("method") << std::endl;
					std::cout << "path: " << client->second->get_request().get_header("path") << std::endl;
					std::cout << "host: " << client->second->get_request().get_header("host") << std::endl;
					this->_servers.at(client->first)->respond(client->second->get_fd());
					this->_pollfds.erase(this->_pollfds.begin() + client->second->get_pollfd_index());
					// seg faults due to trying to loop with iterator after erasing itself
					this->_clients.erase(client);
				}
			}
		}
	}
}
