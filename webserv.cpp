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
#define LOCATION_CONTEXT 3
#define ERR_HTTP_MISSING "Http context is missing."
#define ERR_WRONG_AUTOINDEX "Wrong autoindex value, usage : on | off."
#define ERR_WRONG_AUTOINDEX_ARG "Autoindex, Missing semicolomn ';'."
#define ERR_NEG_BODY_SIZE "client_max_body_size : can't be negative."
#define ERR_BODY_SIZE_ARG "client_max_body_size, Missing semicolomn ';'."
#define ERR_INDEX_ARG "index, Missing semicolomn ';'."
#define ERR_ROOT_ARG "root, Missing semicolomn ';'."
#define ERR_SERVER_BRACKET "server block, Missing opening bracket '{'."
#define ERR_ERROR_PAGE_ARG "error_page, Missing semicolomn ';'."


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
	int 	srv_nb = 0;

    it = vec.begin();
    end = vec.end();
	if (it->compare("http") == 0 && (it + 1)->compare("{") == 0)
		flag = HTTP_CONTEXT;
	else 
		return (error_exit(ERR_HTTP_MISSING));
	it += 2;
	while ( it != end)
	{
	// error_page
		// AUTOINDEX
		if (it->compare("autoindex") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_config.autoindex = (*it);
			else
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX_ARG);
		}
		// CLIENT MAX BODY SIZE
		else if (it->compare("client_max_body_size") == 0 && flag == HTTP_CONTEXT)
		{
		
			it++;
			int 	n = atoi(it->c_str());
			if (n >= 0)
				_config.client_max_body_size = n;
			else
				 throw std::invalid_argument(ERR_NEG_BODY_SIZE);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
		}
		// INDEX
		else if (it->compare("index") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.index = (*it);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
		}
		// ROOT
		else if (it->compare("root") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.root = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
		}
		// ERROR PAGE
		else if (it->compare("error_page") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
			{
				_config.error_page.push_back(*it);
				it++;
			}
			// it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ERROR_PAGE_ARG);
		}
		// SERVER
		else if (it->compare("server") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			if (it->compare("{") != 0)
				throw std::invalid_argument(ERR_SERVER_BRACKET);
			flag = SERVER_CONTEXT;
			t_server *new_server = new t_server;
			_config.server.push_back(*new_server) ;
			srv_nb++;
			it++;

		}
		// LOCATION
		else if (it->compare("location") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			// add location here later
			it++;
			if (it->compare("{") != 0)
				 throw std::invalid_argument(ERR_SERVER_BRACKET);
			flag = LOCATION_CONTEXT;
			it++;
		}
		else if (it->compare("}") == 0 && flag == SERVER_CONTEXT)
		{
			flag = HTTP_CONTEXT;
			it++;
		}

		else if (it->compare("}") == 0 && flag == LOCATION_CONTEXT)
		{
			flag = SERVER_CONTEXT;
			it++;
		}

		else if (it->compare("}") == 0 && flag == HTTP_CONTEXT)
		{
			flag = -1;
			// it = vec.end();
		}
		else if (it->compare("autoindex") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_config.server[srv_nb -1].autoindex = (*it);
			else
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX_ARG);
		}
		else
			it++;


		std::cout << "-----------"  << std::endl;
		std::cout << "token : " << *it << std::endl;
		std::cout << "context : " << flag << std::endl;
	}
		std::cout << "-----------"  << std::endl;
		std::cout << "srv_nb : " << srv_nb << std::endl;
		std::cout << "srv : " << _config.server[0].autoindex  << std::endl;
		std::cout << "srv : " << _config.server[1].autoindex  << std::endl;
		printHttpConfig();
		vec_enum(_config.error_page);
	return;
}
void webserv::printHttpConfig( void )
{
	std::vector<std::string>::iterator it;
	std::cout << "-------------------" << std::endl;
	std::cout << "HTTP config" << std::endl;
	std::cout << "-------------------" << std::endl;
	std::cout << "autoindex : '" << _config.autoindex << "'" << std::endl;
	std::cout << "client_max_body_size : '" << _config.client_max_body_size << "'" << std::endl;
	std::cout << "index : '" << _config.index << "'" << std::endl;
	std::cout << "root : '" <<  _config.root << "'" << std::endl;
	for (it = _config.error_page.begin(); it != _config.error_page.end() ;it++ )
		std::cout << "error_page : '" << *it << "'" << std::endl;
	std::cout << "-------------------" << std::endl;
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