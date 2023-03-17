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
		int i = 0;
		while (true)
		{
			if (i++ == 100)
				exit(0);
			int rc = poll(this->_pollfds.data(), this->_pollfds.size(), 1000);
			if (rc < 0)
				exit(0);
			std::cout << "Poll loop..." << std::endl;
			cout << "Poll size : " << this->_pollfds.size() << endl;
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				if (this->_pollfds[server->second->get_pollfd_index()].revents == 0)
				{
					std::cout << "server revents == 0" << std::endl;
					continue;
				}
					
				cout << "Poll size : " << this->_pollfds.size() << endl;
				std::cout << "Server checking with fd : " << server->second->get_server_fd() << std::endl;
				printf("server revents = %s%s%s at fd %d\n",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLIN) ? "POLLIN" : "",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLOUT) ? "POLLOUT" : "",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLHUP) ? "POLLHUP" : "", server->second->get_server_fd());
				if (this->_pollfds[server->second->get_pollfd_index()].revents & POLLIN)
				{
					int	client_fd = server->second->accept_connection();
					if (client_fd != -1)
					{
						std::cout << "Server pollin..." << std::endl;
						Client*		new_client = new Client(server->second->get_port(), client_fd);
						pollFdType	new_pollfd;

						new_pollfd.fd = client_fd;
						new_pollfd.events = POLLIN;
						this->_pollfds.push_back(new_pollfd);
						// cout << new_client->_pollfd << endl;
						new_client->set_pollfd_index(this->_pollfds.size() - 1);
						this->_clients.push_back(new_client);
						std::cout << "Client created..." << std::endl;
					}
				}
			}
			std::cout << "About to loop through client array of size " << this->_clients.size() << std::endl;
			for (size_t i = 0; i < this->_clients.size(); i++)
			{
				cout << "================" << endl;
				cout << std::boolalpha;
				cout << this->_clients.size() << endl;
				std::cout << "Client loop..." << std::endl;
				std::cout << "pollfd index: " << this->_clients[i]->get_pollfd_index() << std::endl;
				if (this->_pollfds[this->_clients[i]->get_pollfd_index()].revents == 0)
				{
					std::cout << "client at " << this->_clients[i]->get_fd() << " revents == 0" << std::endl;
					cout << "current pollfd " << this->_clients[i]->get_pollfd_index() << std::endl;
				}
				else
				{
					Client*	client = this->_clients[i];
					cout << "Poll size : " << this->_pollfds.size() << endl;
					std::cout << "Client checking with fd : " << client->get_fd() << std::endl;
					printf("client revents = %s%s%s at fd %d\n",
						(this->_pollfds[client->get_pollfd_index()].revents & POLLIN) ? "POLLIN" : "",
						(this->_pollfds[client->get_pollfd_index()].revents & POLLOUT) ? "POLLOUT" : "",
						(this->_pollfds[client->get_pollfd_index()].revents & POLLHUP) ? "POLLHUP" : "", client->get_fd());

					cout << "================" << endl;
					if (this->_pollfds[client->get_pollfd_index()].revents & POLLHUP)
					{
						std::cout << "POLLHUP RECEIVED" << std::endl;
						cout << "Poll size : " << this->_pollfds.size() << endl;
						cout << "Pollfd to be deleted : " << client->get_pollfd_index() << std::endl;
						this->_pollfds.erase(this->_pollfds.begin() + client->get_pollfd_index());
						cout << "Poll size : " << this->_pollfds.size() << endl;
						std::cout << this->_clients.size() << std::endl;
						this->_clients.erase(this->_clients.begin() + i);
						std::cout << this->_clients.size() << std::endl;
					}
					else if (this->_pollfds[client->get_pollfd_index()].revents & POLLIN)
					{
						std::cout << "Client pollin..." << std::endl;
						client->read_buffer();
						if (client->get_request().has_read())
							this->_pollfds[client->get_pollfd_index()].events = POLLOUT;
					}
					else if (this->_pollfds[client->get_pollfd_index()].revents & POLLOUT)
					{
						std::cout << "Client pollout at " << client->get_fd() << std::endl;
						this->_servers[client->get_port()]->respond(client->get_request());
						// this->_pollfds[client->get_pollfd_index()].events = POLLIN;
						this->_pollfds.erase(this->_pollfds.begin() + client->get_pollfd_index());
						this->_clients.erase(this->_clients.begin() + i);
					}
				}
			}
		}
	}
}
