#include "webserv.hpp"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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


void    webser::vec_enum(std::vector<std::string> &vec)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator it2;

    it = vec.begin();
    it2 = vec.end();
	int i = 0;

    std::cout << std::endl;
    while (it != it2) 
	{
        std::cout << "vector[" << i << "] : '" <<*it << "'"<<std::endl;
        it++;
		i++;
    }
}
void    webser::vec_erase_empty(std::vector<std::string> &vec)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator it2;

    it = vec.begin();

    while (++it != vec.end()) 
	{
		if ((*it).empty() == 1 && it != vec.end())
			vec.erase(it);
		if (it == vec.end())
			break;
	}
}

#define HTTP_CONTEXT 1
#define ERR_HTTP_MISSING "Http context is missing."

void webser::error_exit (std::string const & error)
{
	std::cerr << RED;
	std::cerr << "Error : ";
	std::cerr << error;
	std::cerr << std::endl;
	std::cerr << RESET;
	exit (1);
}

void webser::parseToken(std::vector<std::string> & vec)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator end;
	int 	flag = 0;

    it = vec.begin();
    end = vec.end();
	if (it->compare("http") == 0 && (it + 1)->compare("{") == 0)
		flag = HTTP_CONTEXT;
	else 
		return (error_exit(ERR_HTTP_MISSING));
	it += 2;
	while ( it != end)
	{
		autoindex
	client_max_body_size
	index
	error_page
	root 
		if (it->compare("autoindex") == 0)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_auto
				setHttpAutoindex(it + 1, it + 2)
		}
		if (it->compare("http") == 0 && (it + 1)->compare("{") == 0)
			it++;
	}
	return;
}

void webser::tokenizeConfigFile(std::string & src)
{
	std::vector<std::string> token;
	std::string::iterator it = src.begin();
	std::string::iterator end = src.end();;

	size_t i = 0;
	size_t j = 0;

	while (it != end)
	{
		// substring separators ; { }
		if (*it == ';' || *it == '{' || *it == '}')
		{
			token.push_back(src.substr(i, 1));
			j++;
			i++;
			it++;
		}
		// skip spaces
		while (*it == ' ' && it != end)
		{
			it++;
			i++;
			j++;
		}
		// count word len
		while (*it != ' ' && *it !=';' && *it != '{' && *it != '}' && it != end)
		{
			it++;
			j++;
		}
		// substring the word with : (pos, len)
		token.push_back(src.substr(i, j - i));
		i = j;
	}
	// vec_enum(token);
	// std::cout << "------------------" << std::endl;
	vec_erase_empty(token);
	vec_enum(token);
	parseToken(token);
	return ;
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