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
			delete server->second;
		for (clientArrayType::iterator client = this->_clients.begin(); client != this->_clients.end(); client++)
			delete *client;
	}

	bool	WebServer::current_pollfd_is(int event)
	{
		return (this->_pollfds[this->_current_pollfd_index].revents & event);
	}

	void	WebServer::set_current_pollfd_to(int event)
	{
		this->_pollfds[this->_current_pollfd_index].events = event;
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
			// log("Poll loop", "");
			// log("Poll size", this->_pollfds.size());
			this->_current_pollfd_index = 0;
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				// log("Poll size", this->_pollfds.size());
				// log("Server checking with fd", server->second->get_server_fd());
				// printf("server revents = %s%s%s at fd %d\n",
				// 	this->current_pollfd_is(POLLIN) ? "POLLIN" : "",
				// 	this->current_pollfd_is(POLLOUT) ? "POLLOUT" : "",
				// 	this->current_pollfd_is(POLLHUP) ? "POLLHUP" : "", server->second->get_server_fd());

				if (!this->current_pollfd_is(0) && this->current_pollfd_is(POLLIN))
				{
					int	client_fd = server->second->accept_connection();
					if (client_fd != -1)
					{
						// log("Server pollin...", "");
						Client*		new_client = new Client(server->second->get_config(), client_fd);
						pollFdType	new_pollfd;

						new_pollfd.fd = client_fd;
						new_pollfd.events = POLLIN;
						this->_pollfds.push_back(new_pollfd);
						this->_clients.push_back(new_client);
						// log("Client created...", "");
					}
				}
				this->_current_pollfd_index++;
			}
			for (clientArrayType::iterator client = this->_clients.begin(); client != this->_clients.end(); client++)
			{
				// log("Client loop...", "");
				// log("pollfd index", this->_current_pollfd_index);
				if (!this->current_pollfd_is(0))
				{
					// log("Poll size", this->_pollfds.size());
					// printf("client revents = %s%s%s at fd %d\n",
					// 	this->current_pollfd_is(POLLIN) ? "POLLIN" : "",
					// 	this->current_pollfd_is(POLLOUT) ? "POLLOUT" : "",
					// 	this->current_pollfd_is(POLLHUP) ? "POLLHUP" : "", (*client)->get_fd());

					// log("================", "");
					Client*	current_client = (*client);

					if (this->current_pollfd_is(POLLHUP))
					{
						// log("Client pollhup", "");
						// log("current_pollfd_index", this->_current_pollfd_index);
						// log("pollfd_size", this->_pollfds.size());
						this->_pollfds.erase(this->_pollfds.begin() + this->_current_pollfd_index--);
						client = this->_clients.erase(client) - 1;
					}
					else if (this->current_pollfd_is(POLLIN))
					{
						// log("Client pollin", "");
						current_client->handle_request();
						if (current_client->received_request())
							this->set_current_pollfd_to(POLLOUT);
					}
					else if (this->current_pollfd_is(POLLOUT))
					{
						current_client->handle_response();
						if (current_client->sent_response())
							this->set_current_pollfd_to(POLLIN);
					}
				}
				this->_current_pollfd_index++;
			}
		}
	}
}
