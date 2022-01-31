#include "headers.hpp"
#include "colors.hpp"

#define NUM_CLIENTS 100

// * struct pour stocker les fd clients
// ? A remplacer par un vector ou list avec pushback ?
struct client_data {
    int fd;
} clients[NUM_CLIENTS];

int	get_client_socket(int fd) // va cherche le fd et renvoie son index
{
	for (int i = 0; i < NUM_CLIENTS; i++)
		if (clients[i].fd == fd)
			return i;
	return -1;
}

int	add_client_socket(int fd)
{
	int i;
	if ( fd < 1)
		return -1;
	if ((i = get_client_socket(0)) == -1) // je cherche une place dispo dans struct clients
		return -1; // plus de place disponible
	clients[i].fd = fd; // stocke le socket client dans struct clients a l'index récupéré
	return 0;
}

int	del_client_socket(int fd) // supprime le socket_client client, libère sa place et le close
{
	int i;
	if (fd < 1)
		return -1;
	if ((i = get_client_socket(fd)) == -1)
		return -1;
	clients[i].fd = 0;
	return (close(fd));
}

// * pour le test
void send_welcome_msg(int fd)
{
	std::string	s;
	int i = get_client_socket(fd);
	s.append("Yoyyoyoyo! you are client #");
	s.append(std::to_string(i));
	s.append(" !\n");
	send(fd, s.c_str(), s.size(), 0);
}

int	cycle_fd(std::vector<int> listen_sockets, int fd)
{
	for(int i = 0; i < 3; i++)
		if ( fd == listen_sockets[i] )
			return listen_sockets[i];
	return 0;
}


void	listener() // ! kqueue
{
// **************************************************
	int port1 = 18000;
	int port2 = 8200;
	int port3 = 8080;

	std::vector<int> ports;
	ports.push_back(port1);
	ports.push_back(port2);
	ports.push_back(port3);
// **************************************************
	struct timespec	timeout;
	timeout.tv_nsec = 1000;
	timeout.tv_sec = 10;
	char	late[] = "connection timeout !\n";
// **************************************************
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
// **************************************************

	/* 
	*	struct event dans laquelle on va stocker 
	*    struct kevent {
	*    uintptr_t	ident;	     identifier for this event = // ! fd, pid, signal etc
	*    short     filter;	     filter for event = // ! EVFILT_READ or EVFILT_WRITE
	*    u_short   flags;	     action flags for kqueue = // ! EV_ADD | EV_ENABLE pour ajouter // EV_DELETE pour supprimer
	*    u_int     fflags;	     filter flag value
	*    int64_t   data;	     filter data value
	*    void      *udata;	     opaque user data identifier
	*    uint64_t  ext[4];	     extensions
    * };
	*/
	struct kevent	evSet[3]; // events to monitor
	// struct kevent	evSet; // events to monitor
	struct kevent	evList[10]; // events that will be triggered

	// * init kqueue = Creates a new kernel event queue and returns a descriptor
	int	kq = kqueue();

	// * stocke socket serveur dans evSet en mode lecture = on crée un filtre d'évènement a surveiller pour chaque socket (si en lecture),
	// * avec l'action correspondante (add + enable)
	// for( std::vector<int>::iterator it = listen_sockets.begin(), int i = 0; it != listen_sockets.end(), i < 3; it++, i++ )
	for (int i = 0; i < 3; i++)
	{
		EV_SET(&evSet[i],listen_sockets[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		// * on stocke les évènements qui vont être monitoré
		// if (kevent(kq, &(evSet[i]), 1, NULL, 0, NULL) == -1)
		// {
		// 	std::cerr << RED"error: kevent 1 failure\n"RESET;
		// 	return ;
		// }
	}

	if (kevent(kq, evSet, 3, NULL, 0, NULL) == -1)
	{
		std::cerr << RED"error: kevent 1 failure\n"RESET;
		return ;
	}

	struct kevent	evCon; // struct pour stocker les events des sockets clients
	int	client_sock;

	while (1)
	{
		// int time = 0;
		// * equivalent a select
		/*
		* arg 1 = fd event queue to monitor (en read ou write, il faut 1 queue par filtre specifique (fd read, fd write, signal, etc))
		* arg 2 = event a surveiller
		* arg 3 = n event a surveiller
		* arg 4 = event a declencher
		* arg 5 = n event a surveiller
		* arg 6 = timeout
		*/
		std::cout << BLUE"Waiting for connection...\n"RESET;
		int	num_events = kevent(kq, NULL, 0, evList, 10, &timeout);
		std::cout << "timer = " << num_events << std::endl;
		if (num_events == 0)
		{
			for(int i = 0; i < NUM_CLIENTS; i++)
			{
				if (clients[i].fd != 0)
				{
					std::cout << GREEN"client #" << clients[i].fd << " timeout\n";
					send(clients[i].fd, late, strlen(late), 0);
					del_client_socket(clients[i].fd);
					close(clients[i].fd);
				}
			}
		}
		for (int i = 0; i < num_events; i++)
		{
				// Si je trouve le même fd dans evList -> un client est prêt (en read) pour connexion
				if (cycle_fd(listen_sockets, evList[i].ident))
				{
					// * on accepte la connexion : on crée un fd client
					client_sock = accept(evList[i].ident, NULL, NULL);
					if (add_client_socket(client_sock) == 0)
					{
						EV_SET(&evCon, client_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
						kevent(kq, &evCon, 1, NULL, 0, NULL);
						send_welcome_msg(client_sock);
					}
					else
					{
						std::cerr << YELLOW"error: fail to accept connection request\n"RESET;
						close(client_sock);
					}
				 // ! si le client est déconnecté
				 // * flag & type_flag = permet de vérifier si un flag donné est set, l'index permet de désigner le socket client
				 // * le flag EV_EOF indique que la connexion est fermée // EV_ERROR si erreur
				}
				else if (evList[i].flags & EV_EOF)
				{
					std::cout << GREEN"client #" << get_client_socket(evList[i].ident) << " disconnected\n";
					EV_SET(&evCon, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set
					del_client_socket(evList[i].ident);
				}
				else if (evList[i].filter == EVFILT_READ)
				{
					if (!receive_request(evList[i].ident))
					{
						EV_SET(&evCon, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
						kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set
						del_client_socket(evList[i].ident);
					}
				}
				if (num_events == 0)
				{
					std::cout << GREEN"client #" << get_client_socket(evList[i].ident) << " timeout\n";
					send(get_client_socket(evList[i].ident), late, strlen(late), 0);
					EV_SET(&evCon, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
					kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set
					del_client_socket(evList[i].ident);
				}
		}

	}

}













// ! Faire passer ici en arg les spec de configs
void	listener_select()
{
	int port1 = 18000;
	int port2 = 8200;
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
