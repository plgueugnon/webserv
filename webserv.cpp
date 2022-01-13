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
class httpContext
{
	private :
	std::string 			_autoindex;
	std::string 			_client_max_body_size;
	std::string 			_index;
	std::vector<std::string> _error_page;
	std::string _root;

	public :
	httpContext( void );
};


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
			it = line.begin();
			end = line.end();
			// skip spaces
			while ( *it == ' ' || *it == '\t') 
			{
				it++;
				pos++;
			}
			// open curly brace
			if (*it == '{')
			{
				it++;
				pos++;
			}
			while ( *it == ' ' || *it == '\t') 
			{
				it++;
				pos++;
			}
			// check if we can compare 
			//find http
			if ( pos > 4 )
				if ( line.compare(pos, 4, "http") == 0)
				{
					context.assign(line, pos, 4);
					std::cout << "We are in " << context << " context." << std::endl;
				}
			if ( pos > 6 )
				if ( line.compare(pos, 6, "server") == 0)
					context.assign(line, pos, 6);
			if ( end - it > 8 )
				if ( line.compare(pos, 8, "location") == 0)
					context.assign(line, pos, 8);
			if (context.empty() == 0)
				std::cout << context << std::endl;
			// std::cout << line << '\n';
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

	return ;
}
