#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifdef	DEBUG
#define	VERBOSE 1
#else
#define	VERBOSE 0
#endif
 
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

// System headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring> // strtok
#include <string.h>

// Custom headers
#include "webserv.hpp"
#include "colors.hpp"
#include "httpContext.hpp"
#include "cgi.hpp"
#include "listener.hpp"
#include "request.hpp"
#include "response.hpp"
#include "httpCodes.hpp"

#define PORT_MAX 65535
#define PORT_MIN 1

// typedef struct s_request {
// 	std::string	method;
// 	std::string	headers;
// 	std::string	body;

// }	t_request;

// * functions
// void	receive_request(int client_sock);
// void	manage_request(int client_sock, request *request);
void	listener();
int	gen_listen_socket(int port);
int	receive_request(int client_sock);
void	manage_request(int client_sock, t_request *request);


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


// Status Code

// informational
//      100  | Continue                      | Section 6.2.1            |
//    | 101  | Switching Protocols           | Section 6.2.2            |

// success
//    | 200  | OK                            | Section 6.3.1            |
//    | 201  | Created                       | Section 6.3.2            |
//    | 202  | Accepted                      | Section 6.3.3            |
//    | 203  | Non-Authoritative Information | Section 6.3.4            |
//    | 204  | No Content                    | Section 6.3.5            |
//    | 205  | Reset Content                 | Section 6.3.6            |
//    | 206  | Partial Content               | Section 4.1 of [RFC7233] |

// redirection
//    | 300  | Multiple Choices              | Section 6.4.1            |
//    | 301  | Moved Permanently             | Section 6.4.2            |
//    | 302  | Found                         | Section 6.4.3            |
//    | 303  | See Other                     | Section 6.4.4            |
//    | 304  | Not Modified                  | Section 4.1 of [RFC7232] |
//    | 305  | Use Proxy                     | Section 6.4.5            |
//    | 307  | Temporary Redirect            | Section 6.4.7            |

// client error
//    | 400  | Bad Request                   | Section 6.5.1            |
//    | 401  | Unauthorized                  | Section 3.1 of [RFC7235] |
//    | 402  | Payment Required              | Section 6.5.2            |
//    | 403  | Forbidden                     | Section 6.5.3            |
//    | 404  | Not Found                     | Section 6.5.4            |
//    | 405  | Method Not Allowed            | Section 6.5.5            |
//    | 406  | Not Acceptable                | Section 6.5.6            |
//    | 407  | Proxy Authentication Required | Section 3.2 of [RFC7235] |
//    | 408  | Request Timeout               | Section 6.5.7            |
//    | 409  | Conflict                      | Section 6.5.8            |
//    | 410  | Gone                          | Section 6.5.9            |
//    | 411  | Length Required               | Section 6.5.10           |
//    | 412  | Precondition Failed           | Section 4.2 of [RFC7232] |
//    | 413  | Payload Too Large             | Section 6.5.11           |
//    | 414  | URI Too Long                  | Section 6.5.12           |
//    | 415  | Unsupported Media Type        | Section 6.5.13           |
//    | 416  | Range Not Satisfiable         | Section 4.4 of [RFC7233] |
//    | 417  | Expectation Failed            | Section 6.5.14           |
//    | 426  | Upgrade Required              | Section 6.5.15           |

// Server error
//    | 500  | Internal Server Error         | Section 6.6.1            |
//    | 501  | Not Implemented               | Section 6.6.2            |
//    | 502  | Bad Gateway                   | Section 6.6.3            |
//    | 503  | Service Unavailable           | Section 6.6.4            |
//    | 504  | Gateway Timeout               | Section 6.6.5            |
//    | 505  | HTTP Version Not Supported 

#endif // __INCLUDES_H__