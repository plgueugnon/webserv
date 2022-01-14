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

class locationContext
{
	private :
	std::string 					_autoindex;
	std::string 					_client_max_body_size;
	std::string 					_index;
	std::string 					_root;
	std::vector < std::string >		_error_page;
	std::vector < std::string >		_return;
	std::vector < std::string >		_limit_except;
	std::vector < locationContext >	_locationContext;
};

class serverContext
{
	private :
	std::string 					_autoindex;
	std::string 					_client_max_body_size;
	std::string 					_index;
	std::string 					_root;
	std::string 					_server_name;
	std::vector < std::string >		_error_page;
	std::vector < std::string >		_return;
	std::vector < locationContext >	_locationContext;
};

class httpContext
{
	private :
	std::string 					_autoindex;
	std::string 					_client_max_body_size;
	std::string 					_index;
	std::string 					_root;
	std::vector < std::string >		_error_page;
	std::vector < serverContext >	_serverContext;
	std::vector < locationContext >	_locationContext;

	public :
	void 				setHttpAutoindex (std::string const & rhs);
	std::string const & getHttpAutoindex ( void );
};
#endif