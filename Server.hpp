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
#define REQUEST_TIMEOUT 30
#define MONITOR_TIMEOUT_SEC 10
#define MONITOR_TIMEOUT_NSEC 1000

typedef struct	s_set
{
	int				server_socket;
	struct kevent	rset;
	int				port;
}				t_set;

class Server
{
	private:
		/* data */
	public:

		Server( void );
		Server( webserv &server );
		~Server();

		// * server config
		void	setup_config( void );
		int	generate_listen_socket(int port);


		// * client socket monitoring
		unsigned int	gettime(void);
		int	update_client_time(int fd);


		// * server loop
		int	get_client_socket(int fd);
		int	add_client_socket(int fd, int socket_port);
		int	del_client_socket(int fd)
		int	cycle_fd(std::vector<t_set> evSet, int fd);


		// * class var
		int	kq;
		std::vector<int> ports;
		struct timespec	timeout;
		std::vector<t_set> evSet;
		struct kevent	evList[MAX_EVENTS];

		t_client_data	clients[NUM_CLIENTS];
		webserv server_config;

		//* local var
		// struct kevent	evCon;
		// int	client_sock;

		// * temp
		char	late[] = "connection timeout !\n";

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
		return ( "error: socket bind failure\n" );
	}

};

class ListenFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: socket listen failure\n" );
	}

};

class VoidConfig : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: server cannot start without config\n" );
	}

};

#endif // __SERVER_H__