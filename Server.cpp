/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 08:58:17 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/16 19:38:37 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Aincludes.hpp"

std::vector<t_set>	evSet;
t_client_data		clients[NUM_CLIENTS];
bool	stop = false;

// * constructor
Server::Server( void ) { throw VoidConfig(); }
Server::Server( t_http *config ) : server_config(config)
{
	if ( !server_config )
		throw VoidConfig();
	_timeout.tv_nsec = MONITOR_TIMEOUT_NSEC;
	_timeout.tv_sec = MONITOR_TIMEOUT_SEC;
}
// * destructor
Server::~Server() {}

// *______________________________CONFIG____________________________________*

int		Server::generate_listen_socket(int port)
{
		int	server_sock;
		struct sockaddr_in	serv_addr;

		/*
		* TCP only so socket set to stream and not datagram
		* IPv4 config
		*/
		if ( ( server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
			throw SocketCreationFailure();
		serv_addr.sin_family = AF_INET;

		/*
		* No subject requirements to manage specific IPs so any IP is accepted
		*/
		serv_addr.sin_addr.s_addr = INADDR_ANY;

		if ( port < PORT_MIN || port > PORT_MAX )
			throw InvalidPort();
		serv_addr.sin_port = htons(port);

		/*
		* bind an IP, a port with a fd to create a socket
		*/
		if ( ( bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) ) < 0 )
		{
			std::cerr << RED"Failure with port : " << port << "\n"RESET;
			throw BindFailure();
		}

		/*
		* set socket in listening mode
		*/
		if ( (listen(server_sock, 1024)) < 0 )
			throw ListenFailure();
		return ( server_sock );
}

void	Server::setup_config( void )
{
	evSet.resize(server_config->server.size());
	for (size_t i = 0; i < evSet.size(); i++)
	{
		int p = atoi(server_config->server[i].listen.c_str());
		evSet[i].server_socket = generate_listen_socket(p);
		evSet[i].port = p;
		evSet[i].server = i;
		if (VERBOSE)
			std::cout << GREEN"Port: " << evSet[i].port << " succesfully set\n"RESET;
	}
	_kq = kqueue();

	// * we store in fd _kq our server sockets on read only mode
	for (size_t i = 0; i < evSet.size(); i++)
	{
		EV_SET(&evSet[i].rset, evSet[i].server_socket, EVFILT_READ, EV_ADD, 0, 0, 0);
		if (kevent(_kq, &(evSet[i].rset), 1, NULL, 0, NULL) == -1)
			throw KeventFailure();
	}

	for (int i = 0; i < NUM_CLIENTS; i++)
	{
		clients[i].fd = 0;
		clients[i].port = 0;
		clients[i].time = 0;
		clients[i].server = 0;
		clients[i].timeout = false;
	}
}

// *____________________________SERVER_LOOP_________________________________*

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		if (VERBOSE)
			std::cout << GREEN"Shutting down server\n"RESET;
		for (int i = 0; i < NUM_CLIENTS; i++)
			close(clients[i].fd);
		for (size_t i = 0; i < evSet.size(); i++)
			close(evSet[i].server_socket);
		stop = true;
	}
}

void	Server::answer_client(int client_sock, std::string answer)
{

	// int	send_rc;
	// int	send_left = (int)answer.size();
	// char *message_ptr = strdup(answer.c_str());

	// while (send_left > 0)
	// {
	// 	send_rc = send(client_sock, message_ptr, send_left, 0);
	// 	if (send_rc == -1)
	// 		break ;
	// 	send_left -= send_rc;
	// 	message_ptr += send_rc;
	// }
	// if (client_sock == 0)
	// {
	// 	std::cerr << RED"error: " << client_sock << " invalid fd\n"RESET;
	// 	return ;
	// }
	// TODO A modifier pour découper envoie selon un buffer => max = 1mb
	int r = send(client_sock, answer.c_str(), answer.size(), 0);
	switch (r)
	{
		case -1:
			std::cerr << RED"error: failure to send answer\n"RESET;
			del_client_socket(client_sock);
			break;
		case 0:
			if (VERBOSE)
				std::cout << YELLOW"warning: no data sent\n"RESET;
			break;
		default:
			break;
	}		
	return ;
}

void	Server::update_events(int fd, int update)
{
	switch (update)
	{
		case add_read :
			EV_SET(&_evCon, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
			if (kevent(_kq, &_evCon, 1, NULL, 0, NULL) < 0)
				std::cerr << RED"error: kevent failure to set READ event\n"RESET;
			break;

		case read_to_write :
			EV_SET(&_evCon, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
			if (kevent(_kq, &_evCon, 1, NULL, 0, NULL) < 0)
				std::cerr << RED"error: kevent failure to delete READ event\n"RESET;
			EV_SET(&_evCon, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
			if (kevent(_kq, &_evCon, 1, NULL, 0, NULL) < 0)
				std::cerr << RED"error: kevent failure to set READ to WRITE event\n"RESET;
			break;

		case add_write :
			EV_SET(&_evCon, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
			if (kevent(_kq, &_evCon, 1, NULL, 0, NULL) < 0)
				std::cerr << RED"error: kevent failure to set WRITE event\n"RESET;
			break;

		default:
			std::cerr << RED"error: update_events unrecognized flag parameter\n"RESET;
			break;
	}
}

void	Server::manage_request(t_client_data *client, t_server config)
{
	response 	response(*client->request, config);
	// std::pair <bool, bool> ready (false, false);

	response.setLocation();
	response.setRoot();
	response.setPath();
	response.setIndex();
	signal(SIGINT, &sighandler);
	if ( (client->request->requestLine[request::METHOD]).compare(0, 4, "POST") == 0 &&
			response.isBodyTooLarge() == false )
	{
			// struct kevent	fdlist[MAX_EVENTS];
			// int n;
			// std::cout << "CHEEEEEECK\n";
			if (pipe(client->read_fd) < 0 || pipe(client->write_fd) < 0)
				throw PipeFailure();
			update_events(client->write_fd[1], add_write);
			update_events(client->read_fd[0], add_read);
			// std::cout << "write fd is " << client->write_fd[1] << std::endl;
			response.setCGIfd(client->read_fd, client->write_fd); // TODO intégrer les filtres READ ET WRITE ici ou intégrer dans boucle ou rajouter les fonctions de handlePost ici
			// while (1)
			// {
			// // 	n = kevent(_kq, NULL, 0, fdlist, MAX_EVENTS, &_timeout);
			// 	// std::cout << "events " << n << std::endl;
			// 	for (int i = 0; i < n; i++)
			// 	{
			// 		if (fdlist[i].filter == EVFILT_READ)
			// 		{
			// 			// std::cout << "read " << std::endl;
			// 			ready.first = true;
			// 		}
			// 		else if (fdlist[i].filter == EVFILT_WRITE)
			// 		{
			// 			// std::cout << "write " << std::endl;
			// 			ready.second = true;
			// 		}
			// 	}
				// if ((ready.first && ready.second) || n == 0)
				// {
				// 	std::cout << "check ready\n";
				// 	response.parse();
				// 	break ;
				// }
			// }
			response.parse();
			// std::cout << ready.first << " | " << ready.second << std::endl;
	}
	else
	// std::cout << "check manage request = " << client->request->requestLine[request::METHOD] << "\n";
		response.parse();
	client->answer = response.ret;
}

int	Server::receive_request(t_client_data *client, t_server config)
{
	ssize_t n = 0;
	char buffer[BUFFER_SIZE];

	while ( (n = recv(client->fd, &buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[n] = '\0';
		if (client->request->isBody == false)
		{
			client->request->buf += buffer;
			client->request->parseHeader();
			if (client->request->body.size() == (unsigned long)atoi(client->request->header[request::CONTENT_LENGTH].c_str()))
				client->request->BodyReady = true;
		}
		else
		{
			client->request->buf = buffer;
			client->request->redirectBody();
			if (client->request->body.size() == (unsigned long)atoi(client->request->header[request::CONTENT_LENGTH].c_str()))
				client->request->BodyReady = true;
		}
		if (n < BUFFER_SIZE - 1)
			break ;
	}
	if (n == 0)
	{
		if (VERBOSE)
			std::cout << YELLOW"warning: no data received\n"RESET;
		return 0;
	}
	if ((client->request->BodyReady == true && client->request->requestLine[request::METHOD].compare(0,4,"POST") == 0) ||
		(client->request->isBody == true && client->request->requestLine[request::METHOD].compare(0,4,"POST") != 0))
	{
			// std::cout << BOLDCYAN"I'M IN!\n"RESET;
			// std::cout << MAGENTA << client->request->isBody << RESET << std::endl;
			// std::cout << MAGENTA << client->request->BodyReady << RESET << std::endl;
			// std::cout << MAGENTA << "size received " << client->request->body.size() << RESET << std::endl;
			// std::cout << MAGENTA << "size target " << client->request->header[request::CONTENT_LENGTH] << RESET << std::endl;
			manage_request(client, config);
			return 1;
	}
	else
	{
		// std::cout << CYAN << client->request->isBody << RESET << std::endl;
		// std::cout << CYAN << client->request->BodyReady << RESET << std::endl;
		// std::cout << CYAN << "size received " << client->request->body.size() << RESET << std::endl;
		// std::cout << CYAN << "size target " << client->request->header[request::CONTENT_LENGTH] << RESET << std::endl;
		return 2;
	}
	// return 1;
}

unsigned int	Server::gettime(void)
{
	long	time;
	struct timeval	ctime;

	time = 0;
	if (gettimeofday(&ctime, NULL))
		std::cerr << RED"Error: fail to get time\n"RESET;
	time = ctime.tv_sec * 1000 + ctime.tv_usec / 1000;
	return (time);
}

int	Server::get_client_socket(int fd)
{
	for (int i = 0; i < NUM_CLIENTS; i++)
		if (clients[i].fd == fd)
			return i;
	return -1;
}

int	Server::add_client_socket(int fd, int socket_port, int server)
{
	request	request;
	int i;
	if ( fd < 1)
		return -1;
	if ((i = get_client_socket(0)) == -1)
		return -1;
	clients[i].fd = fd;
	clients[i].time = gettime() + (REQUEST_TIMEOUT * 1000);
	clients[i].port = socket_port;
	clients[i].server = server;
	clients[i].timeout = false;
	clients[i].request = new class request;
	if (VERBOSE)
		std::cout << GREEN"client #" << get_client_socket(clients[i].fd) << " with fd #" << clients[i].fd << " and port " << socket_port << "\n"RESET;
	return 0;
}

int	Server::update_client_time(int fd)
{
	int i;
	if (fd < 1)
		return 0;
	if ((i = get_client_socket(fd)) == -1)
		return 0;
	clients[i].time = gettime() + (REQUEST_TIMEOUT * 1000);
	return 1;
}

int	Server::del_client_socket(int fd)
{
	int i;
	if (fd < 1)
		return -1;
	if ((i = get_client_socket(fd)) == -1)
		return -1;
	clients[i].fd = 0;
	clients[i].time = 0;
	clients[i].port = 0;
	clients[i].server = 0;
	clients[i].timeout = false;
	delete clients[i].request;
	// close(clients[i].read_fd[0]);
	// close(clients[i].read_fd[1]);
	// close(clients[i].write_fd[0]);
	// close(clients[i].write_fd[1]);
	// clients[i].request = NULL;
	if (VERBOSE)
		std::cout << GREEN"Closing connection with client #" << i << "\n"RESET;
	return (close(fd));
}

int	Server::cycle_fd(std::vector<t_set> evSet, int fd)
{
	for(size_t i = 0; i < evSet.size(); i++)
		if ( fd == evSet[i].server_socket )
			return i;
	return -1;
}

void	Server::clear_late_clients( void )
{
	unsigned int now = gettime();
	for(int i = 0; i < NUM_CLIENTS; i++)
	{
		if (clients[i].fd)
		{
			if (clients[i].time - now <= 0)
			{
				if (VERBOSE)
					std::cout << GREEN"client #" << get_client_socket(clients[i].fd) << " on port " << clients[i].port << " with fd #" << clients[i].fd << " timeout\n"RESET;
				update_events(clients[i].fd, read_to_write);
				clients[i].timeout = true;
			}
		}
	}
}

void	Server::run( void )
{
	int	client_sock;
	int	num_events;
	int	n;

	signal(SIGINT, &sighandler);
	if (VERBOSE)
		std::cout << GREEN"Server up and ready...\n"RESET;
	while (1)
	{
		if (VERBOSE)
			std::cout << BLUE"Waiting for connection...\n"RESET;
		if ((num_events = kevent(_kq, NULL, 0, _evList, MAX_EVENTS, &_timeout)) == 0)
			clear_late_clients();
		for (int i = 0; i < num_events; i++)
		{
			if ((n = cycle_fd(evSet, _evList[i].ident)) != -1)
			{
				client_sock = accept(_evList[i].ident, NULL, NULL);
				if (add_client_socket(client_sock, evSet[n].port, evSet[n].server) == 0)
					update_events(client_sock, add_read);
				else
				{
					std::cerr << YELLOW"error: fail to accept connection request on port: " << evSet[n].port << "\n"RESET;
					del_client_socket(_evList[i].ident);
				}
			}
			else if (_evList[i].flags & EV_ERROR)
			{
				if (VERBOSE)
					std::cout << YELLOW"client #" << get_client_socket(_evList[i].ident) << " disconnected due to error: " << _evList[i].data << "\n";
				del_client_socket(_evList[i].ident);
			}
			else if (_evList[i].flags & EV_EOF)
			{
				if (VERBOSE)
					std::cout << GREEN"client #" << get_client_socket(_evList[i].ident) << " disconnected\n";
				del_client_socket(_evList[i].ident);
			}
			else if (_evList[i].filter == EVFILT_READ)
			{
				int r = get_client_socket(_evList[i].ident);
				// if (!receive_request(&clients[r], server_config->server[clients[r].server]))
				// {
				// 	std::cout << "echec\n";
				// 	del_client_socket(_evList[i].ident);
				// }
				// else
				// 	update_events(clients[r].fd, EVFILT_WRITE);
				int n = receive_request(&clients[r], server_config->server[clients[r].server]);
				switch (n)
				{
					case 0:
						del_client_socket(_evList[i].ident);
						break ;
					case 1:
						update_events(clients[r].fd, read_to_write);
						break ;
					case 2:
						// std::cout << CYAN << _evList[i].data << RESET << std::endl;
						// std::cout << "Not all is received\n";
						break ;
				}
			}
			else if (_evList[i].filter == EVFILT_WRITE)
			{
				// int r;
				// if ((r = get_client_socket(_evList[i].ident)) < 0 )
				// {
				// 	std::cout << "FUCK YOU " << _evList[i].ident << std::endl;
				// 	close(_evList[i].ident);
				// }
				int r = get_client_socket(_evList[i].ident);
				// else
				// {
					if (clients[r].timeout)
					{
						clients[r].answer = CODE_408;
						clients[r].answer += "\nConnection: close";
						clients[r].answer += CRLF;
						clients[r].answer += "<html><body><h1>Too late buddy !</h1></body></html>";
					}
					// std::cout << "this is the fd " << clients[r].fd << std::endl;
					answer_client(clients[r].fd, clients[r].answer);
					del_client_socket(_evList[i].ident);
				// }
			}
		}
		if (stop)
			break ;
	}
}

