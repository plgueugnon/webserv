/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 08:58:17 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/14 13:21:04 by pgueugno         ###   ########.fr       */
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

void	Server::answer_client(int client_sock, std::string answer)
{
	if (send(client_sock, answer.c_str(), answer.size(), 0) == -1)
	{
		std::cerr << YELLOW"error: failure to send answer\n"RESET;
		// close(client_sock);
		del_client_socket(client_sock);
		return ;
	}
}

void	Server::update_events(int fd, int update)
{
	switch (update)
	{
		case EVFILT_READ :
			EV_SET(&_evCon, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			kevent(_kq, &_evCon, 1, NULL, 0, NULL);
			break;

		case EVFILT_WRITE :
			EV_SET(&_evCon, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
			EV_SET(&_evCon, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
			kevent(_kq, &_evCon, 2, NULL, 0, NULL);
			break;

		default:
			std::cerr << RED"error: update_events failure\n"RESET;
			break;
	}
}

void	Server::manage_request(t_client_data *client, request *request, t_server config)
{
	response 	response(*request, config);
	if ( (request->requestLine[request::METHOD]).compare("POST") == 0 )
	{
		pipe(client->read_fd);
		pipe(client->write_fd);
		update_events(client->read_fd[0], EVFILT_READ);
		// update_events(client->read_fd[1], EVFILT_WRITE);
		update_events(client->write_fd[0], EVFILT_READ);
		// update_events(client->write_fd[1], EVFILT_WRITE);
		response.setCGIfd(client->read_fd, client->write_fd);
	}
	response.parse();
	// answer += response.ret;
	client->answer = response.ret;
}


int	Server::receive_request(t_client_data *client, t_server config)
{
	ssize_t n = 0;
	char buffer[BUFFER_SIZE];
	request 		request;

	// ! Keep-alive request pas traité par défaut
	// ? A implémenter ?
	while ( (n = recv(client->fd, &buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[n] = '\0';
		if (request.isBody == false)
		{
			request.buf += buffer;
			request.parseHeader();
		}
		else
		{
			request.buf = buffer;
			request.redirectBody();
		}

		if (n == 0 || n == EAGAIN )
			return 0;
		if (n < BUFFER_SIZE - 1)
			break;
	}
	manage_request(client, &request, config);
	return 1;
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
	int i;
	if ( fd < 1)
		return -1;
	if ((i = get_client_socket(0)) == -1)
		return -1;
	clients[i].fd = fd;
	clients[i].time = gettime() + (REQUEST_TIMEOUT * 1000); // ! chaque requete commence avec 30 sec (nginx = 60)
	clients[i].port = socket_port;
	clients[i].server = server;
	clients[i].timeout = false;
	if (VERBOSE)
		std::cout << GREEN"client # " << get_client_socket(clients[i].fd) << "with fd #" << clients[i].fd << " and port " << socket_port << "\n"RESET;
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
		// std::cout << "check time ?\n";
		if (clients[i].fd)
		{
			std::cout << "client #" << i << " time left = " << clients[i].time - now << "\n"; // ! A SUPPR
			if (clients[i].time - now <= 0)
			{
				if (VERBOSE)
					std::cout << GREEN"client # " << get_client_socket(clients[i].fd) << " on port " << clients[i].port << " with fd # " << clients[i].fd << " timeout\n"RESET;
				// send(clients[i].fd, late, strlen(late), 0); // ! A modifier pour faire une reponse 408 timeout au client
				update_events(clients[i].fd, EVFILT_WRITE); // TODO modif pour passer en write client avec un signal 408
				clients[i].timeout = true;
				// del_client_socket(clients[i].fd); // TODO suppr ici pour fermer requete après envoi reponse seulement
			}
		}
	}
}

void	sighandler(int signum)
{
	if (signum == SIGINT)
	{
		if (VERBOSE)
			std::cout << GREEN"Shutting down server\n"RESET;
		for (int i = 0; i < NUM_CLIENTS; i++)
			// shutdown(clients[i].fd, SHUT_RDWR);
			close(clients[i].fd);
		for (size_t i = 0; i < evSet.size(); i++)
			// shutdown(evSet[i].server_socket, SHUT_RDWR);
			close(evSet[i].server_socket);
		stop = true;
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
				if (add_client_socket(client_sock, evSet[n].port, evSet[n].server) == 0) // ! 1 event a la fois read or write // pas poss les 2 en meme temps
				{
					update_events(client_sock, EVFILT_READ);
				}
				else
				{
					std::cerr << YELLOW"error: fail to accept connection request on port: " << evSet[n].port << "\n"RESET;
					close(client_sock);
				}
			}
			else if (_evList[i].flags & EV_ERROR)
			{
				if (VERBOSE)
					std::cout << YELLOW"client #" << get_client_socket(_evList[i].ident) << " disconnected due to error: " << _evList[i].data << "\n";
				del_client_socket(_evList[i].ident); // ! Pas  besoin check si read ou write -> close() elimine fd et tous events associés
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
				if (!receive_request(&clients[r], server_config->server[clients[r].server]))
				{
					del_client_socket(_evList[i].ident);
				}
				else
					update_events(clients[r].fd, EVFILT_WRITE); 
			}
			else if (_evList[i].filter == EVFILT_WRITE)
			{
				int r = get_client_socket(_evList[i].ident);
				if (clients[r].timeout)
				{
					clients[r].answer = CODE_408;
					clients[r].answer += CRLF;
				}
				answer_client(clients[r].fd, clients[r].answer);
				del_client_socket(_evList[i].ident);
			}
		}
		if (stop)
			break ;
	}
}

