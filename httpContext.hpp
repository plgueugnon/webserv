/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef HTTPCONTEXT_HPP
#define HTTPCONTEXT_HPP

#include "colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef struct s_location
{
	std::string 					autoindex;
	size_t							client_max_body_size;
	std::string 					index;
	std::string 					root;
	std::vector < std::string >		error_page;
	std::vector < std::string >		return_dir;
	std::vector < std::string >		limit_except;
	std::vector < s_location >		locationContext;

} 				t_location;

// class locationContext
// {
// 	private :
// 	std::string 					_autoindex;
// 	// size_t							_client_max_body_size;
// 	std::string 					_index;
// 	std::string 					_root;
// 	std::vector < std::string >		_error_page;
// 	std::vector < std::string >		_return;
// 	std::vector < std::string >		_limit_except;
// 	std::vector < locationContext >	_locationContext;
// };

typedef struct s_server
{
	std::string 					autoindex;
	size_t							client_max_body_size;
	std::string 					index;
	std::string 					root;
	std::string 					server_name;
	std::vector < std::string >		error_page;
	std::vector < std::string >		return_dir;
	std::vector < t_location >		location;
} 				t_server;

// class serverContext
// {
// 	private :
// 	std::string 					_autoindex;
// 	// size_t							_client_max_body_size;
// 	std::string 					_index;
// 	std::string 					_root;
// 	std::string 					_server_name;
// 	std::vector < std::string >		_error_page;
// 	std::vector < std::string >		_return;
// 	std::vector < locationContext >	_locationContext;
// };

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

// class httpContext
// {
// 	// private :
// 	std::string 					_autoindex;
// 	size_t							_client_max_body_size;
// 	std::string 					_index;
// 	std::string 					_root;
// 	std::vector < std::string >		_error_page;
// 	std::vector < serverContext >	_serverContext;
// 	std::vector < locationContext >	_locationContext;

// 	public :
// 	// SET
// 	void 				setHttpAutoindex (std::string const & rhs);
// 	void 				setHttpClientMaxBodySize (size_t const & rhs);
// 	void 				setHttpIndex (std::string const & rhs);
// 	void 				setHttpRoot (std::string const & rhs);
// 	void 				setHttpErrorPage (std::string const & rhs);

// 	// GET
// 	std::string const & getHttpAutoindex ( void );
// 	size_t 		const & getHttpClientMaxBodySize ( void );
// 	std::string const & getHttpIndex ( void );
// 	std::string const & getHttpRoot ( void );
// std::vector<std::string> & getHttpErrorPage( void );
// std::vector<serverContext> & getHttpServerContext( void );


// 	void 				printHttpConfig ( void );
// };
#endif