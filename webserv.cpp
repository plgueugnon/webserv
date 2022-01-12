#include "webserv.hpp"
#include <iostream>
#include <fstream>
#include <string>

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
 * DIRECTIVE 				| CONTEXT
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

 */

void webserv::parseConfigFile ( void )
{
	std::string line;
	std::ifstream file;

	file.open(_file_name );

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::cout << line << '\n';
		}
		file.close();
	}
  else
	  std::cout << "Unable to open file";

	return ;
}
