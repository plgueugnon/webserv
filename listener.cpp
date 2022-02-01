#include "Aincludes.hpp"

// * https://www.nginx.com/blog/tuning-nginx/
// ! nginx accepts by default 512 connections per worker
// * as we are not authorized to use fork to create separate workers, max number of connections
// * is therefore set to nginx max default value

#define NUM_CLIENTS 512
#define MAX_EVENTS 10
#define REQUEST_TIMEOUT 30
#define MONITOR_TIMEOUT_SEC 10
#define MONITOR_TIMEOUT_NSEC 1000


typedef struct s_set
{
	struct kevent	rset;
	int				port;
}	t_set;

unsigned int	gettime(void)
{
	long	time;
	struct timeval	ctime;

	time = 0;
	if (gettimeofday(&ctime, NULL))
		std::cerr << RED"Error: fail to get time\n"RESET;
	time = ctime.tv_sec * 1000 + ctime.tv_usec / 1000;
	return (time);
}


// * struct pour stocker les fd clients
// ? A remplacer par un vector ou list avec pushback ?
struct client_data 
{
	int		fd;
	long	time;
	int		port;
} clients[NUM_CLIENTS];

int	get_client_socket(int fd) // * va cherche le fd et renvoie son index
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
	if ((i = get_client_socket(0)) == -1) // * je cherche une place dispo dans struct clients
		return -1; // * plus de place disponible
	clients[i].fd = fd; // * stocke le socket client dans struct clients a l'index récupéré
	clients[i].time = gettime() + (REQUEST_TIMEOUT * 1000); // ! chaque requete commence avec 30 sec (nginx = 60)
	return 0;
}

int	update_client_time(int fd)
{
	int i;
	if (fd < 1)
		return 0;
	if ((i = get_client_socket(fd)) == -1)
		return 0;
	clients[i].time = gettime() + (REQUEST_TIMEOUT * 1000);
	return 1;
}

int	del_client_socket(int fd) // * supprime le socket_client client, libère sa place et le close
{
	int i;
	if (fd < 1)
		return -1;
	if ((i = get_client_socket(fd)) == -1)
		return -1;
	clients[i].fd = 0;
	clients[i].time = 0;
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
	for(size_t i = 0; i < listen_sockets.size(); i++)
		if ( fd == listen_sockets[i] )
			return listen_sockets[i];
	return 0;
}

// TODO Rajouter class/struct avec serveur, port et socket pour yann

void	listener(webserv *server) // ! kqueue
{
	std::vector<int> ports = server->listenPorts;
// **************************************************
	// int port1 = 18000;
	// int port2 = 8200;
	// int port3 = 8080;

	// std::vector<int> ports;
	// ports.push_back(port1);
	// ports.push_back(port2);
	// ports.push_back(port3);
// **************************************************
	struct timespec	timeout;
	timeout.tv_nsec = MONITOR_TIMEOUT_NSEC;
	timeout.tv_sec = MONITOR_TIMEOUT_SEC;
	char	late[] = "connection timeout !\n";
// **************************************************
	// ! je stocke les sockets d'écoute dans un tableau
	std::vector<int> listen_sockets; // TODO AJOUTER ici port dans t_set pour chaque listen socket
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

	std::vector<t_set>      evSet(3); // * events to monitor
	struct kevent	evList[MAX_EVENTS]; // * events that will be triggered

	// * init kqueue = Creates a new kernel event queue and returns a descriptor
	int	kq = kqueue();

	// * stocke socket serveur dans evSet en mode lecture = on crée un filtre d'évènement a surveiller pour chaque socket (si en lecture),
	// * avec l'action correspondante (add + enable)
	for (int i = 0; i < 3; i++)
	{
		EV_SET(&evSet[i].rset,listen_sockets[i], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		// * on stocke les évènements qui vont être monitoré
		if (kevent(kq, &(evSet[i].rset), 1, NULL, 0, NULL) == -1)
		{
			std::cerr << RED"error: kevent 1 failure\n"RESET;
			return ;
		}
	}

	struct kevent	evCon; // * struct pour stocker les events des sockets clients
	int	client_sock;

	// * Listening loop
	while (1)
	{
		// * equivalent a select
		std::cout << BLUE"Waiting for connection...\n"RESET;
		int	num_events = kevent(kq, NULL, 0, evList, 10, &timeout);
		if (num_events == 0)
		{
			unsigned int now = gettime();
			for(int i = 0; i < NUM_CLIENTS; i++)
			{
				if (clients[i].fd != 0)
				{
					std::cout << "client #" << i << " time left = " << clients[i].time - now << "\n";
					if (clients[i].time - now <= 0)
					{
						std::cout << GREEN"client #" << get_client_socket(clients[i].fd) << "with fd #" << clients[i].fd << " timeout\n"RESET;

						send(clients[i].fd, late, strlen(late), 0); // ! A modifier pour faire une reponse 408 timeout au client
						close(clients[i].fd);
						for (int j = 0; j < MAX_EVENTS; j++)
						{
							if (evList[j].ident == (unsigned long)clients[i].fd)
							{
								std::cout << "entry test for client # " << i << " or " << j << std::endl;
								EV_SET(&evCon, evList[j].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL); // TODO RAjouter un kevent en write sur le meme fd
								kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set
								break ;
							}
						}
						del_client_socket(clients[i].fd);
					}
				}
			}
		}
		for (int i = 0; i < num_events; i++)
		{
				// * Si je trouve le même fd dans evList -> un client est prêt (en read) pour connexion
				if (cycle_fd(listen_sockets, evList[i].ident))
				{
					// * on accepte la connexion : on crée un fd client
					client_sock = accept(evList[i].ident, NULL, NULL);
					if (add_client_socket(client_sock) == 0) // ! NB A verifier : ajout de 1 seul socket mais avec 2 events
					{
						EV_SET(&evCon, client_sock, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL); // TODO RAjouter un kevent en write sur le meme fd
						kevent(kq, &evCon, 1, NULL, 0, NULL); // TODO augmenter a 2
						// send_welcome_msg(client_sock);
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
					EV_SET(&evCon, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);  // TODO RAjouter un kevent en write sur le meme fd
					kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set  // TODO augmenter a 2
					del_client_socket(evList[i].ident); 
				}
				else if (evList[i].filter == EVFILT_READ)
				{
					int r = get_client_socket(evList[i].ident);
					std::cout << "client #" << r << " old time = " << clients[r].time << "\n";
					if (!receive_request(clients[r].fd, server->_config))
					{
						EV_SET(&evCon, evList[i].ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);  // TODO RAjouter un kevent en write sur le meme fd
						kevent(kq, &evCon, 1, NULL, 0, NULL); // actualise le fd set  // TODO augmenter a 2
						del_client_socket(evList[i].ident);
					}
					else
					{
						update_client_time(evList[i].ident);
						// int j = get_client_socket(evList[i].ident);
						// std::cout << "client #" << j << " updated time = " << clients[j].time << " with fd #" << clients[j].fd << "\n";
					}
				}
		}

	} // TODO Ajouter boucle de fermeture des sockets clients puis serveur en cas de signal
		// TODO tester avec shutdown(clients[i].fd, SHUT_RDWR);
}
