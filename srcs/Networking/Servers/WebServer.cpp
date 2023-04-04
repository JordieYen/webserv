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
		signal(SIGPIPE, SIG_IGN);
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
				std::cout << "Error : poll returns error..." << std::endl;
			this->_current_pollfd_index = 0;
			for (serverMapType::iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
			{
				if (!this->current_pollfd_is(0) && this->current_pollfd_is(POLLIN))
				{
					int	client_fd = server->second->accept_connection();
					if (client_fd != -1)
					{
						Client*		new_client = new Client(*(server->second), client_fd);
						pollFdType	new_pollfd;

						new_pollfd.fd = client_fd;
						new_pollfd.events = POLLIN;
						this->_pollfds.push_back(new_pollfd);
						this->_clients.push_back(new_client);
					}
				}
				this->_current_pollfd_index++;
			}
			for (clientArrayType::iterator client = this->_clients.begin(); client != this->_clients.end(); client++)
			{
				if (!this->current_pollfd_is(0))
				{
					Client*	current_client = (*client);

					if (this->current_pollfd_is(POLLHUP))
					{
						close(this->_pollfds[this->_current_pollfd_index].fd);
						this->_pollfds.erase(this->_pollfds.begin() + this->_current_pollfd_index--);
						delete *client;
						client = this->_clients.erase(client) - 1;
					}
					else if (this->current_pollfd_is(POLLIN))
					{
						current_client->handle_request();
						if (current_client->received_request())
							this->set_current_pollfd_to(POLLOUT);
					}
					else if (this->current_pollfd_is(POLLOUT))
					{
						current_client->handle_response();
						if (current_client->sent_response())
						{
							// this->set_current_pollfd_to(POLLIN);
							close(this->_pollfds[this->_current_pollfd_index].fd);
							this->_pollfds.erase(this->_pollfds.begin() + this->_current_pollfd_index--);
							delete *client;
							client = this->_clients.erase(client) - 1;
						}
					}
				}
				this->_current_pollfd_index++;
			}
		}
	}
}
