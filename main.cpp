#include <iostream>
#include <stdlib.h>
#include "colors.hpp"
#include "webserv.hpp"
#include "headers.hpp"

void print_welcome( void )
{
	std::cout << BOLDCYAN;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "O  WEBSERV BY PILOU & YANNOU <3  O" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << std::endl;
	std::cout << RESET;
	return ;
}

void usage_too_many_args ( void )
{
	std::cout << RED;
	std::cerr << "To many arguments provided. 1 argument max." << std::endl ;
	std::cerr << "Usage : ./webserv 'path/config_file'."<< std::endl;
	std::cout << RESET;
	exit (1);
}

void usage_one_arg ( void )
{
	std::cout << GREEN;
	std::cout << "No argument provided." << std::endl;
	std::cout << "Default configuration file will be used." << std::endl;
	std::cout << "You can edit file at 'etc/webserv.conf'." << std::endl;
	std::cout << RESET;
	return ;
}

void usage_two_args ( char * str )
{
	std::cout << GREEN;
	std::cout << "Loading '" << str << "'";
	std::cout << " as configuration file."<< std::endl;
	std::cout << RESET;
	return ;
}

int main (int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	// webserv		server;

	// print_welcome();

	// if (ac > 2)
	// 	usage_too_many_args();
	// else
	// {
	// 	if (ac == 1)
	// 	{
	// 		usage_one_arg();
	// 		server.setFileName("etc/webserv.conf");
	// 	}
	// 	if (ac == 2)
	// 	{
	// 		usage_two_args(av[1]);
	// 		server.setFileName(av[1]);
	// 	}
	// }
	// std::cout << server.getFileName() << std::endl;

	int port1 = 18000;
	int port2 = 820;
	int port3 = 8080;

	std::vector<int> ports;
	ports.push_back(port1);
	ports.push_back(port2);
	ports.push_back(port3);

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
		listen_sockets.push_back(gen_listen_socket(*it));
	// ! puis je stocke les sockets dans une struct read_fs
	for( std::vector<int>::iterator it = listen_sockets.begin(); it != listen_sockets.end(); it++ )
	{
		std::cout << "fd = " << *it << "\n";
		FD_SET(*it, &read_fs);
	}

	std::vector<unsigned char> buf(4096);
	ssize_t n = 0;
	while (true)
	{
		// ! necessaire de réactualiser le reader_fd a chaque tour pour acceder à un nouveau port
		reader = read_fs;
		std::cout << "Waiting for connection...\n";
		if (select(FD_SETSIZE, &reader, NULL, NULL, NULL) < 0)
		{
			std::cerr << "error: select failure : " << errno << "\n";
			return -1;
		}

		for(std::vector<int>::iterator it = listen_sockets.begin(); it != listen_sockets.end(); it++ )
			if ( FD_ISSET(*it, &reader) )
			{
				if ((client_sock = accept(*it, NULL, NULL)) == -1)
				{
					std::cerr << "error: fail to accept connection request\n";
					return -1;
				}
				std::cout << "New client connection...\n";
			}


		if ( (n = recv(client_sock, buf.data(), 4096 - 1, 0)) > 0)
		{
			std::cout << buf.data() << std::endl;
			if (n == 0 || n == EAGAIN)
				break;
		}
		if (n < 0)
		{
			std::cerr << "error: failure to receive request\n";
			close(client_sock);
			return -1;
		}
		char answer[] = "HTTP/1.1 200 OK\r\n\r\n Hello !";
		if (send(client_sock, answer, std::strlen(answer), 0) == -1)
		{
			std::cerr << "error: failure to send answer\n";
			close(client_sock);
			return -1;
		}
		close(client_sock);
	}




	return (1);
}
