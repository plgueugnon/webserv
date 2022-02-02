#include "Aincludes.hpp"

// * constructor
Server::Server( void ) { throw VoidConfig(); }
Server::Server( webserv *config ) : server_config(config)
{
	if ( !server_config )
		throw VoidConfig();
	_timeout.tv_nsec = MONITOR_TIMEOUT_NSEC;
	_timeout.tv_sec = MONITOR_TIMEOUT_SEC;
}
// * destructor
Server::~Server() {}

// *______________________________CONFIG____________________________________*\\

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
	evSet.resize(server_config->listenPorts.size());
	for (size_t i = 0; i < evSet.size(); i++)
	{
		evSet[i].server_socket = generate_listen_socket(server_config->listenPorts[i]);
		evSet[i].port = server_config->listenPorts[i];
		if (VERBOSE)
			std::cout << GREEN"Port " << evSet[i].port << "succesfully set\n"RESET;
	}
	_kq = kqueue();

	for (size_t i = 0; i < evSet.size(); i++)
	{
		EV_SET(&evSet[i].rset, evSet[i].server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		// * we store in fd _kq server sockets on read mode
		if (kevent(_kq, &(evSet[i].rset), 1, NULL, 0, NULL) == -1)
			throw KeventFailure();
	}
}

// *____________________________SERVER_LOOP_________________________________*\\

void	Server::run( void )
{
	struct kevent	evCon;
	int				client_sock;
	int				num_events;




}


