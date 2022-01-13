#include "webserv.hpp"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

webserv::webserv ( void )
{ return ;}

void webserv::setFileName (std::string name)
{
	_file_name.assign(name);
}

std::string const & webserv::getFileName ( void ) const
{
	return _file_name ;
}

/*
 * LIST OF CONTEXTS to implement
 * - server
 * - http
 * - location
 *
 * LIST OF DIRECTIVES to implement
 * DIRECTIVE 				| CONTEXT possible
 * --------------------------------------
 * - http 					| N/A
 * - server					| http
 * - autoindex 				| http / server / location
 * - client_max_body_size 	| http / server / location
 * - index 				 	| http / server / location
 * - error_page 		 	| http / server / location
 * - root 		 			| http / server / location
 * - location			 	| server / location
 * - return				 	| server / location
 * - listen 	 			| server
 * - server_name 			| server
 * - limit_except 			| location
 *
 * find CONTEXT -> always between '{}'
 * find http context : http string and {}
 * find server context : server string and {}
 * find location context : location string and {}
 * 
 * find DIRECTIVE
 * 
 * find VALUE
 */

/*
 http directives :
	server
	autoindex
	client_max_body_size
	index
	error_page
	root 
*/

/*
 server directives :
	autoindex
	client_max_body_size
	index
	error_page
	root 
	location
	return
	server_name
*/

/*
 location directives :
	autoindex
	client_max_body_size
	index
	error_page
	root 
	location
	return
	limit_except
*/

class serverContext;
class locationContext;
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
	httpContext( void );
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

	public :
	serverContext ( void );
};

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

	public :
	locationContext ( void ) ;
};

void    vec_enum(std::vector<std::string> &vec)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator it2;

    it = vec.begin();
    it2 = vec.end();
	int i = 0;

    std::cout << std::endl;
    while (it != it2) 
	{
        std::cout << "vector[" << i << "] : " <<*it << std::endl;
        it++;
		i++;
    }
}
void tokenizeConfigFile(std::string & src)
{
	(void)src;
	std::vector<std::string> token;

	std::string str;

	std::string::iterator it = src.begin();
	std::string::iterator end = src.end();;

	size_t i = 0;
	size_t j = 0;

	while (it != end)
	{
		while (*it == ' ' && it != end)
		{
			it++;
			i++;
			j++;
		}
		while (*it != ' ' && it != end)
		{
			it++;
			j++;
		}
		std::cout << "i: " << i << "| j: " << j << std::endl;
		str = src.substr(i, j);
		std::cout << "'" << str << "'" << std::endl;
		std::cout << "------------------" << std::endl;
		// token.push_back(str);
		i = j;
	}
	// vec_enum(token);
	
	// std::cout << src << std::endl;

}

void webserv::parseConfigFile ( void )
{
	std::string config_string = "";
	std::string line;
	std::ifstream file;
	std::vector<std::string> 	token;

	file.open(_file_name );

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line[0] != '#')
				config_string += line;
		}
		file.close();
		tokenizeConfigFile(config_string);
	}
  else
  {
	  std::cerr << "Unable to open '";
	  std::cerr << _file_name;
	  std::cerr << "' file." << std::endl;
	  std::cerr << RESET;
  }

	return ;
}