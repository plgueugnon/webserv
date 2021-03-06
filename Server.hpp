/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 08:59:15 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/16 18:46:35 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_H__
#define __SERVER_H__

#include "Aincludes.hpp"

/* 
* https://www.nginx.com/blog/tuning-nginx/
! nginx accepts by default 512 connections per worker
* as we are not authorized to use fork to create separate workers, max number of connections
* is therefore set to nginx max default value
*/ 

#define NUM_CLIENTS 512
#define MAX_EVENTS 100

/*
* each request begins with a 30 sec timeout delay
* the monitoring loop will check client timeout every 10 sec
*/
#define REQUEST_TIMEOUT 30
#define MONITOR_TIMEOUT_SEC 10
#define MONITOR_TIMEOUT_NSEC 100000

// * recv buffer size
#define BUFFER_SIZE 4096

typedef enum	e_events
{
	add_read,
	add_write,
	read_to_write,
	del_write,
	del_read,
}				t_events;

typedef struct	s_set
{
	int				server_socket;
	struct kevent	rset;
	int				port;
	int				server;
	bool			timeout;
}				t_set;

class Server
{
	public:

		Server( void );
		Server( t_http *server );
		~Server();

		// * server config
		void	setup_config(void);
		int	generate_listen_socket(int port);

		// * client socket monitoring
		unsigned int	gettime(void);
		int	update_client_time(int fd);

		// * server loop
		void	update_events(int fd, int update);
		int	get_client_socket(int fd);
		int	add_client_socket(int fd, int socket_port, int server);
		int	del_client_socket(int fd);
		void	clear_late_clients(void);
		int	cycle_fd(std::vector<t_set> evSet, int fd);
		void	run(void);

		// * client I/O
		void	answer_client(int client_sock, std::string answer);
		void	manage_request(t_client_data *client, t_server config);
		int	receive_request(t_client_data *client, t_server config);

		// * attributes
	private:
		int				_kq;
		struct timespec	_timeout;
		struct kevent	_evList[MAX_EVENTS];
		struct kevent	_evCon;

	public:
		t_http			*server_config;

};

class SocketCreationFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: socket creation failure\n" );
	}

};

class InvalidPort : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: invalid port number\n" );
	}

};

class BindFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: bind failure\n" );
	}

};

class ListenFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: listen failure\n" );
	}

};

class VoidConfig : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: server cannot start without config\n" );
	}

};

class KeventFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: kevent failure\n" );
	}

};

class PipeFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: pipe creation failure\n" );
	}

};

#endif // __SERVER_H__