/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef HTTPCONTEXT_HPP
#define HTTPCONTEXT_HPP

#include "colors.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct s_location
{
	std::string 					path;
	std::string 					autoindex;
	size_t							client_max_body_size;
	std::string 					index;
	std::string 					root;
	std::vector < std::string >		error_page;
	std::vector < std::string >		return_dir;
	std::vector < std::string >		limit_except;
	// std::vector < s_location >		location;

} 				t_location;

typedef struct s_server
{
	std::string 					autoindex;
	size_t							client_max_body_size;
	std::string 					index;
	std::string 					root;
	std::string 					server_name;
	std::string 					listen;
	std::vector < std::string >		error_page;
	std::vector < std::string >		return_dir;
	std::vector < s_location >		location;
} 				t_server;

typedef struct s_http
{
	std::string 					autoindex;
	size_t							client_max_body_size;
	std::string 					index;
	std::string 					root;
	std::vector < std::string >		error_page;
	std::vector < t_server >		server;
	std::vector < t_location >		location;
}				t_http;

#endif