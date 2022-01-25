#include "headers.hpp"
#include "colors.hpp"

// ! Faire passer ici en arg les spec de configs
void	listener(std::vector<int> ports)
{
	// int port1 = 18000;
	// int port2 = 820;
	// int port3 = 8080;

	// std::vector<int> ports;
	// ports.push_back(port1);
	// ports.push_back(port2);
	// ports.push_back(port3);

	// ! fd_set de lecture -> pour déterminer si le socket server est prêt pour lecture
	fd_set	read_fs;
	// ! obligatoire d'init la struct avec FD_ZERO pour pouvoir y stocker des sockets
	FD_ZERO(&read_fs);
	// fd_set	write_fs;
	int	client_sock;

	fd_set	reader = { 0 };

	// ! je stocke les sockets d'écoute dans un tableau
	std::vector<int> listen_sockets;
	for( std::vector<int>::iterator it = ports.begin(); it != ports.end(); it++ )
	{
		try
		{
			listen_sockets.push_back(gen_listen_socket(*it));
		}
		catch ( const std::exception &e )
		{
			std::cerr << e.what();
			return ;
		}
	}

	// ! puis je stocke les sockets dans une struct read_fs
	for( std::vector<int>::iterator it = listen_sockets.begin(); it != listen_sockets.end(); it++ )
		FD_SET(*it, &read_fs);

	// * Listening loop
	while (true)
	{
		// ! necessaire de réactualiser le reader_fd a chaque tour pour acceder à un nouveau port
		reader = read_fs;
		if (VERBOSE)
			std::cout << BLUE"Waiting for connection...\n"RESET;
		if (select(FD_SETSIZE, &reader, NULL, NULL, NULL) < 0)
		{
			std::cerr << RED"error: select failure : " << errno << "\n"RESET;
			return ;
		}

		for(std::vector<int>::iterator it = listen_sockets.begin(); it != listen_sockets.end(); it++ )
			if ( FD_ISSET(*it, &reader) )
			{
				if ((client_sock = accept(*it, NULL, NULL)) == -1)
				{
					std::cerr << RED"error: fail to accept connection request\n"RESET;
					return ;
				}
				if (VERBOSE)
					std::cout << GREEN"New client connection...\n"RESET;
			}

		receive_request(client_sock);
	}
	// TODO Ajouter boucle de fermeture des sockets serveur
	// ? Capture du signal pour terminer programme ? Doit être géré ?
}
