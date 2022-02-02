#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifdef	DEBUG
#define	VERBOSE 1
#else
#define	VERBOSE 0
#endif

// * global fixed variables
#define PORT_MAX 65535
#define PORT_MIN 1

typedef struct s_client_data
{
	int		fd;
	long	time;
	int		port;
}	t_client_data;

// * stream header
#include <iostream>
#include <fstream>

// * socket header
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/event.h>

// * time header
#include <sys/time.h>

// * basic header
#include <string>
#include <vector>
#include <signal.h>
#include <unistd.h>

// * System headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring> // strtok
#include <string.h>

// * Custom headers
#include "httpContext.hpp"
#include "webserv.hpp"
#include "cgi.hpp"
#include "request.hpp"
#include "response.hpp"
#include "httpCodes.hpp"
#include "Server.hpp"

// * Utility headers
#include "colors.hpp"
#include "utils.hpp"


// * functions (temporaire jusqu'a creation de classe)

void	listener(webserv *server);
int		gen_listen_socket(int port);

// void	receive_request(int client_sock);
// void	manage_request(int client_sock, request *request);
// void	listener();
// int	gen_listen_socket(int port);
// int	receive_request(int client_sock);
// void	manage_request(int client_sock, t_request *request);


// request line
// Method
// path
// query=?
// protocol

// : separateur clé / contenu

// request header
// Authorization
// Accept
// Accept-Charset
// Accept-Encoding
// Accept-Language
// Content-Type
// Content-Length
// Date
// Host = IP/DNS:PORT
// Referer
// Transfer-Encoding
// User-Agent


// response header
// Allow
// Content-Language
// Content-Length
// Content-Location
// Content-Type
// Date
// Last-Modified
// Location
// Retry-After
// Server
// Transfer-Encoding
// WWW-Authenticate


#endif // __INCLUDES_H__