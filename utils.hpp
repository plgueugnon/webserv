/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

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
#include "headers.hpp"
#include "listener.hpp"
#include "request.hpp"
#include "response.hpp"
#include "httpCodes.hpp"

#define PORT_MAX 65535
#define PORT_MIN 1


// print all nodes of a vector
void    vec_enum(std::vector<std::string> &vec);
void    vec_enum(std::vector<int> &vec);
// erase empty token in the vector
void    vec_erase_empty(std::vector<std::string> &vec);
void    print_env_c(const char ** env);

bool     is_error_code ( int  code);
bool     onlyDigits ( const char *  str);


#endif /* UTILS_HPP */
