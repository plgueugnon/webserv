#include "Aincludes.hpp"

// * default constructor
Server::Server( void ) 
{
	throw VoidConfig();
}

Server::Server( webserv &server ) : server_config(server); 
{
	if ( !server_config )
		throw VoidConfig();
	
}

// * destructor
Server::~Server() {}

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

}

void	Server::setup_config( void )
{

}
