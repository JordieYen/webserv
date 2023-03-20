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
			int rc = poll(this->_pollfds.data(), this->_pollfds.size(), 1000);
			if (rc < 0)
				exit(0);
			log("Poll loop", "");
			log("Poll size", this->_pollfds.size());
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				if (this->_pollfds[server->second->get_pollfd_index()].revents == 0)
				{
					log("server revents == 0", "");
					continue;
				}
					
				log("Poll size", this->_pollfds.size());
				log("Server checking with fd", server->second->get_server_fd());
				printf("server revents = %s%s%s at fd %d\n",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLIN) ? "POLLIN" : "",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLOUT) ? "POLLOUT" : "",
					(this->_pollfds[server->second->get_pollfd_index()].revents & POLLHUP) ? "POLLHUP" : "", server->second->get_server_fd());

				if (this->_pollfds[server->second->get_pollfd_index()].revents & POLLIN)
				{
					int	client_fd = server->second->accept_connection();
					if (client_fd != -1)
					{
						log("Server pollin...", "");
						Client*		new_client = new Client(server->second->get_port(), client_fd);
						pollFdType	new_pollfd;

						new_pollfd.fd = client_fd;
						new_pollfd.events = POLLIN;
						this->_pollfds.push_back(new_pollfd);
						new_client->set_pollfd_index(this->_pollfds.size() - 1);
						this->_clients.push_back(new_client);
						log("Client created...", "");
					}
				}
			}
			for (clientArrayType::iterator client = this->_clients.begin(); client != this->_clients.end();)
			{
				log("Client loop...", "");
				log("pollfd index", (*client)->get_pollfd_index());
				if (this->_pollfds[(*client)->get_pollfd_index()].revents == 0)
				{
					log("client revents == 0, fd", (*client)->get_fd());
					log("current pollfd", (*client)->get_pollfd_index());
					client++;
				}
				else
				{
					log("Poll size", this->_pollfds.size());
					log("Client checking with fd", (*client)->get_fd());
					printf("client revents = %s%s%s at fd %d\n",
						(this->_pollfds[(*client)->get_pollfd_index()].revents & POLLIN) ? "POLLIN" : "",
						(this->_pollfds[(*client)->get_pollfd_index()].revents & POLLOUT) ? "POLLOUT" : "",
						(this->_pollfds[(*client)->get_pollfd_index()].revents & POLLHUP) ? "POLLHUP" : "", (*client)->get_fd());

					log("================", "");
					if (this->_pollfds[(*client)->get_pollfd_index()].revents & POLLHUP)
					{
						log("Client pollhup", "");
						this->_pollfds.erase(this->_pollfds.begin() + (*client)->get_pollfd_index());
						client = this->_clients.erase(this->_clients.begin() + i);
					}
					else if (this->_pollfds[(*client)->get_pollfd_index()].revents & POLLIN)
					{
						log("Client pollin", "");
						(*client)->read_buffer();
						if ((*client)->get_request().has_read())
							this->_pollfds[(*client)->get_pollfd_index()].events = POLLOUT;
						client++;
					}
					else if (this->_pollfds[(*client)->get_pollfd_index()].revents & POLLOUT)
					{
						log("Client pollout at", (*client)->get_fd());
						this->_servers[(*client)->get_port()]->respond((*client)->get_request());
						(*client)->clear_buffer();
						this->_pollfds[(*client)->get_pollfd_index()].events = POLLIN;
						client++;
					}
				}
			}
		}
	}
}
