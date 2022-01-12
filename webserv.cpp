#include "webserv.hpp"
#include "colors.h"
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
 * find CONTEXT
 * find http context : http string and {}
 * find server context : server string and {}
 * find location context : location string and {}
 * 
 * find DIRECTIVE
 * 
 * find VALUE
 */

void webserv::parseConfigFile ( void )
{
	std::string line;
	std::string context;
	std::ifstream file;
	std::string::iterator it;
	std::string::iterator end;
	size_t 		pos = 0;

	file.open(_file_name );

	if (file.is_open())
	{
		// file get line to the end of the file
		while (getline(file, line))
		{
			//find http
			it = line.begin();
			end = line.end();
			while ( *it == ' ')///* || line[pos] == '\t') */ && line[pos] != '\0' )
				it++;
			if ( end - it > 4 )
			{
			if ( line.compare(pos, 4, "http") == 0)
				context.assign(line, pos, 4);
			}
			if (context.empty() == 0)
				std::cout << context << std::endl;
			std::cout << line << '\n';
			context.clear();
		}
		file.close();
	}
  else
  {
	  std::cerr << "Unable to open '";
	  std::cerr << _file_name;
	  std::cerr << "' file." << std::endl;
	  std::cerr << RESET;
  }
			std::cout << "test" << std::endl;

	return ;
}
