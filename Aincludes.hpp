/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Aincludes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 08:58:43 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/14 09:54:31 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#define CRLF "\r\n\r\n"

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
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <cstdio> // remove (file)
#include <cstring> // strtok
#include <string.h>
#include <dirent.h> // opendir

// * client struct
typedef struct s_client_data
{
	int			fd;
	long		time;
	int			port;
	int			server;
	std::string	answer;
	bool		timeout;
	int			write_fd[2];
	int			read_fd[2];
}	t_client_data;

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

#endif // __INCLUDES_H__