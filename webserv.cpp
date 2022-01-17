#include "webserv.hpp"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

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
        std::cout << "vector[" << i << "] : '" <<*it << "'"<<std::endl;
        it++;
		i++;
    }
}
void    vec_erase_empty(std::vector<std::string> &vec)
{
    std::vector<std::string>::iterator it = vec.begin();

    while (++it != vec.end()) 
	{
		if ((*it).empty() == 1 && it != vec.end())
			vec.erase(it);
		if (it == vec.end())
			break;
	}
}

#define HTTP_CONTEXT 1
#define SERVER_CONTEXT 2
#define LOCATION_CONTEXT 2
#define ERR_HTTP_MISSING "Http context is missing."
#define ERR_WRONG_AUTOINDEX "Wrong autoindex value, usage : on | off."
#define ERR_WRONG_AUTOINDEX_ARG "Autoindex, Missing semicolomn ';'."
#define ERR_NEG_BODY_SIZE "client_max_body_size : can't be negative."
#define ERR_BODY_SIZE_ARG "client_max_body_size, Missing semicolomn ';'."
#define ERR_INDEX_ARG "index, Missing semicolomn ';'."
#define ERR_ROOT_ARG "root, Missing semicolomn ';'."


void error_exit (std::string const & error)
{
	std::cerr << RED;
	std::cerr << "Error : ";
	std::cerr << error;
	std::cerr << std::endl;
	std::cerr << RESET;
	exit (1);
}

void webserv::parseToken(std::vector<std::string> & vec)
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
	// index
	// error_page
	// root 
		// AUTOINDEX
		if (it->compare("autoindex") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_config.setHttpAutoindex(*it);
			else
				return (error_exit(ERR_WRONG_AUTOINDEX));
			it++;
			if (it->compare(";") != 0)
				return (error_exit(ERR_WRONG_AUTOINDEX_ARG));
		}
		// CLIENT MAX BODY SIZE
		else if (it->compare("client_max_body_size") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			int 	n = atoi(it->c_str());
			if (n >= 0)
				_config.setHttpClientMaxBodySize(n);
			else
				return (error_exit(ERR_NEG_BODY_SIZE));
			it++;
			if (it->compare(";") != 0)
				return (error_exit(ERR_BODY_SIZE_ARG));
		}
		// INDEX
		else if (it->compare("index") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.setHttpIndex(*it);
			it++;
			if (it->compare(";") != 0)
				return (error_exit(ERR_INDEX_ARG));
		}
		// ROOT
		else if (it->compare("root") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.setHttpRoot(*it);
			it++;
			if (it->compare(";") != 0)
				return (error_exit(ERR_ROOT_ARG));
		}
		it++;
	}
		_config.printHttpConfig();
	return;
}

void webserv::tokenizeConfigFile(std::string & src)
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