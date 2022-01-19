#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <iostream>
#include <string>
#include <vector>

#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SERVER_PORT 80
#define MAXLINE 4096

#define RST		"\033[0m"
#define RED		"\033[0;31m"
#define BLU		"\033[0;34m"
#define YEL		"\033[0;33m"

#define SA struct sockaddr_in

#endif // __CLIENT_H__