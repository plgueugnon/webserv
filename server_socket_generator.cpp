#include "Aincludes.hpp"

class SocketCreationFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: failure to create socket\n" );
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
		return ( "error: failure to bind\n" );
	}

};

class ListenFailure : public std::exception {

public:

	virtual const char	*what( void ) const throw() {
		return ( "error: failure to listen socket\n" );
	}

};

int	gen_listen_socket(int port)
{
	int	server_sock;
	// int	client_sock;
	struct sockaddr_in	serv_addr;

	// ! TCP only so socket set to stream and not datagram
	if ( ( server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		throw SocketCreationFailure();

	// ! IPv4 config
	serv_addr.sin_family = AF_INET;

	// TODO Le sujet ne demande pas plusieurs IP a voir si on implemente ça
	// ! par défaut toute IP est acceptée
	// serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// ! Recupérer ici port indiqué dans fichier de config
	// ! Verifie que le port est bien viable avant d'en config l'endianess
	if ( port < 0 || port > 65535 )
		throw InvalidPort();
	serv_addr.sin_port = htons(port);

	// int enable = 1;
	// if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	// 	std::cerr << "error : setsockopt failure\n";
	// fcntl(server_sock, F_SETFL, O_NONBLOCK);

	// ! associe le IP et port a un fd pour créer un socket d'écoute
	if ( ( bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) ) < 0 )
	{
		std::cerr << RED"Failure with port : " << port << "\n"RESET;
		throw BindFailure();
	}

	// ! place la nouvelle socket créé en état d'écoute
	if ( (listen(server_sock, 1024)) < 0 )
		throw ListenFailure();

	// ! renvoie la nouvelle socket ainsi créée
	return ( server_sock );
}

// 1. Check arg entry main
// Puis depuis main
// 2. Parse fichier de config
// ! -> stockage Class Webserv
// ! -> Check erreur syntaxe simple ( mot clé valide ? )
// 3. Init listen socket pour chaque port (I = int port / O = socket fd (1 par port))
// 4. Lancement boucle écoute (I = liste des ports / O = tableau de string avec les requetes entrantes )
// 5. Parsing des requetes et reponses