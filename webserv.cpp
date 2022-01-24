/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:40:54 by ygeslin           #+#    #+#             */
/*   Updated: 2022/01/24 14:37:46 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// System headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

// Custom headers
#include "utils.hpp"

// Parsing context flags to parse the token (AST)
#define HTTP_CONTEXT 1
#define SERVER_CONTEXT 2
#define LOCATION_CONTEXT 3

// Error exception messages
#define ERR_HTTP_MISSING "Http context is missing."
#define ERR_WRONG_AUTOINDEX "Wrong autoindex value, usage : on | off."
#define ERR_WRONG_AUTOINDEX_ARG "Autoindex, Missing semicolomn ';'."
#define ERR_NEG_BODY_SIZE "client_max_body_size : can't be negative."
#define ERR_BODY_SIZE_ARG "client_max_body_size, Missing semicolomn ';'."
#define ERR_INDEX_ARG "index, Missing semicolomn ';'."
#define ERR_ROOT_ARG "root, Missing semicolomn ';'."
#define ERR_LISTEN_ARG "listen, Missing semicolomn ';'."
#define ERR_SERVER_NAME_ARG "server_name, Missing semicolomn ';'."
#define ERR_SERVER_BRACKET "server block, Missing opening bracket '{'."
#define ERR_LOCATION_BRACKET "location block, Missing opening bracket '{'."
#define ERR_LOCATION_SLASH "location route must begin by '/'."
#define ERR_ERROR_PAGE_ARG "error_page, Missing semicolomn ';'."
#define ERR_LIMIT_EXCEPT_ARG "except_limit, Missing semicolomn ';'."
#define ERR_RETURN_ARG "except_limit, Missing semicolomn ';'."
#define ERR_WRONG_DIRECTIVE "unknown directive or open brackets"
#define ERR_WRONG_PORT_RANGE "wrong port range, min 0, max 65 535."

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
{ 
	bzero(&_config, sizeof(t_http));
	return ;
}

void webserv::setFileName (std::string name)
{
	_file_name.assign(name);
}

// return config file name / path
std::string const & webserv::getFileName ( void ) const
{
	return _file_name ;
}

// return an empty location struct to push_back in the vectors
// To avoid allocate and use "new"
t_location newLocation ( void )
{
	t_location loc;

	bzero(&loc, sizeof(t_location));
	return (loc);
}

// return an empty server struct to push_back in the vectors
// To avoid allocate and use "new"
t_server newServer ( void )
{
	t_server srv;

	bzero(&srv, sizeof(t_server));
	return (srv);
}

// Parse token and fill data structures according to context
void webserv::parseToken(std::vector<std::string> & vec)
{
	// ! PENSER A LIMITER LE NOMBRE DE TOKEN AVANT LES ";"
	// ! FAIRE LE CHECK DES VALEURS DU FICHIER DE CONFIG, du genre
	// ! si port 80 pas dispo, quitter, si code erreur non connu etc
	// ! donnees indispensables non renseignees
    std::vector<std::string>::iterator	it;
    std::vector<std::string>::iterator	end;
	std::string 						tmp;

	int 								flag = 0;
	int 								srv_nb = -1;
	int 								loc_nb = -1;

    it = vec.begin();
    end = vec.end();
	if (it->compare("http") == 0 && (it + 1)->compare("{") == 0)
		flag = HTTP_CONTEXT;
	else
		throw std::invalid_argument(ERR_HTTP_MISSING);
	it += 2;
	while ( it != end)
	{
		// ! -----------HTTP CONTEXT------------ START
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
			it++;
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
			it++;
		}
		// INDEX
		else if (it->compare("index") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.index = (*it);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
			it++;
		}
		// ROOT
		else if (it->compare("root") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			_config.root = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
			it++;
		}
		// ERROR PAGE
		else if (it->compare("error_page") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
				_config.error_page.push_back(*it++);
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ERROR_PAGE_ARG);
			it++;
		}
		// SERVER
		else if (it->compare("server") == 0 && flag == HTTP_CONTEXT)
		{
			it++;
			if (it->compare("{") != 0)
				throw std::invalid_argument(ERR_SERVER_BRACKET);
			flag = SERVER_CONTEXT;
			_config.server.push_back(newServer()) ;
			srv_nb++;
			it++;

		}
		else if (it->compare("}") == 0 && flag == HTTP_CONTEXT)
		{
			flag = -1;
			it++;
		}
		// ! ----------- HTTP CONTEXT ------------ END

		// ! ----------- SERVER CONTEXT ------------ BEGINING
		// LOCATION
		else if (it->compare("location") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			tmp = *it;
			if (tmp[0] != '/')
				 throw std::invalid_argument(ERR_LOCATION_SLASH);
			// add location here later
			it++;
			if (it->compare("{") != 0)
				 throw std::invalid_argument(ERR_LOCATION_BRACKET);
			flag = LOCATION_CONTEXT;
			_config.server[srv_nb].location.push_back(newLocation()) ;
			loc_nb++;
			_config.server[srv_nb].location[loc_nb].path = tmp;
			it++;
		}
		else if (it->compare("}") == 0 && flag == SERVER_CONTEXT)
		{
			flag = HTTP_CONTEXT;
			loc_nb = -1;
			it++;
		}

		else if (it->compare("}") == 0 && flag == LOCATION_CONTEXT)
		{
			flag = SERVER_CONTEXT;
			it++;
		}

		else if (it->compare("autoindex") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_config.server[srv_nb].autoindex = (*it);
			else
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX_ARG);
			it++;
		}
		else if (it->compare("client_max_body_size") == 0 && flag == SERVER_CONTEXT)
		{
		
			it++;
			int 	n = atoi(it->c_str());
			if (n >= 0)
				_config.server[srv_nb].client_max_body_size = n;
			else
				 throw std::invalid_argument(ERR_NEG_BODY_SIZE);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
			it++;
		}
		// INDEX
		else if (it->compare("index") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			_config.server[srv_nb].index = (*it);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
			it++;
		}
		// ROOT
		else if (it->compare("root") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			_config.server[srv_nb].root = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
			it++;
		}
		// LISTEN
		else if (it->compare("listen") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			_config.server[srv_nb].listen = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
			it++;
		}
		// SERVER NAME
		else if (it->compare("server_name") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			_config.server[srv_nb].server_name = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
			it++;
		}
		// ERROR PAGE
		else if (it->compare("error_page") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
			{
				_config.server[srv_nb].error_page.push_back(*it);
				it++;
			}
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ERROR_PAGE_ARG);
			it++;
		}
		// ! ----------- SERVER CONTEXT ------------ END

		// ! ----------- LOCATION CONTEXT ------------ BEGINING
		// LOCATION AUTOINDEX
		else if (it->compare("autoindex") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			if (it->compare("on") == 0 || it->compare("off") == 0 )
				_config.server[srv_nb].location[loc_nb].autoindex = (*it);
			else
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_WRONG_AUTOINDEX_ARG);
			it++;
		}
		else if (it->compare("client_max_body_size") == 0 && flag == LOCATION_CONTEXT)
		{
		
			it++;
			int 	n = atoi(it->c_str());
			if (n >= 0)
				_config.server[srv_nb].location[loc_nb].client_max_body_size = n;
			else
				 throw std::invalid_argument(ERR_NEG_BODY_SIZE);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_BODY_SIZE_ARG);
			it++;
		}
		// INDEX
		else if (it->compare("index") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			_config.server[srv_nb].location[loc_nb].index = (*it);
			it++;
			if (it->compare(";") != 0)
				 throw std::invalid_argument(ERR_INDEX_ARG);
			it++;
		}
		// ROOT
		else if (it->compare("root") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			_config.server[srv_nb].location[loc_nb].root = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ROOT_ARG);
			it++;
		}
		// ERROR PAGE
		else if (it->compare("error_page") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
			{
				_config.server[srv_nb].location[loc_nb].error_page.push_back(*it);
				it++;
			}
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_ERROR_PAGE_ARG);
			it++;
		}
		// LIMITE EXCEPT
		else if (it->compare("limit_except") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
				_config.server[srv_nb].location[loc_nb].limit_except.push_back(*it++);
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_LIMIT_EXCEPT_ARG);
			it++;
		}
		// RETURN	
		else if (it->compare("return") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
			{
				_config.server[srv_nb].return_dir.push_back(*it);
				it++;
			}
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_RETURN_ARG);
			it++;
		}
		// ! ----------- LOCATION CONTEXT ------------ END
		else
		{

			// print erreur ici si bracket pas fermee ou directive inconnue
			it--;
		std::cout << "-----------"  << std::endl;
		std::cout << "token -1 : " << *it << std::endl;
		std::cout << "context -1 : " << flag << std::endl;
			it++;
		std::cout << "-----------"  << std::endl;
		std::cout << "token : " << *it << std::endl;
		std::cout << "context : " << flag << std::endl;
			it++;
			// return ;
			throw std::invalid_argument(ERR_WRONG_DIRECTIVE);

		}


	}
	if (VERBOSE)
		printHttpConfig();
	return;
}

void printLocationConfig ( std::vector< t_location> & loc)
{
	std::vector<t_location>::iterator	loc_it;
	std::vector<std::string>::iterator	it;

	int 								loc_nb = 1;

	for (loc_it = loc.begin(); loc_it != loc.end(); loc_it++)
	{
		std::cout << MAGENTA;
		std::cout << "---------------------" << std::endl;
		std::cout << " Location config " << loc_nb << std::endl;
		std::cout << "---------------------" << std::endl;
		std::cout << "path : \t\t\t'" << loc_it->path << "'" << std::endl;
		std::cout << "autoindex : \t\t'" << loc_it->autoindex << "'" << std::endl;
		std::cout << "client_max_body_size :\t'" << loc_it->client_max_body_size << "'" << std::endl;
		std::cout << "index : \t\t'" << loc_it->index << "'" << std::endl;
		std::cout << "root : \t\t\t'" << loc_it->root << "'" << std::endl;
		// error pages
		for (it = loc_it->error_page.begin(); it != loc_it->error_page.end(); it++)
			std::cout << "error_page : \t\t'" << *it << "'" << std::endl;
		// limit except
		for (it = loc_it->limit_except.begin(); it != loc_it->limit_except.end(); it++)
			std::cout << "limit_except : \t\t'" << *it << "'" << std::endl;
		loc_nb++;
	}
	std::cout << RESET;
	return ;
}

void webserv::printServerConfig ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<std::string>::iterator	it;

	int 								srv_nb = 1;

	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		std::cout << BLUE;
		std::cout << "-----------------------------" << std::endl;
		std::cout << "\tServer config " << srv_nb << std::endl;
		std::cout << "-----------------------------" << std::endl;
		std::cout << "listen : \t\t'" << srv_it->listen << "'" << std::endl;
		std::cout << "server_name : \t\t'" << srv_it->server_name << "'" << std::endl;
		std::cout << "autoindex : \t\t'" << srv_it->autoindex << "'" << std::endl;
		std::cout << "client_max_body_size :\t'" << srv_it->client_max_body_size << "'" << std::endl;
		std::cout << "index : \t\t'" << srv_it->index << "'" << std::endl;
		std::cout << "root : \t\t\t'" << srv_it->root << "'" << std::endl;
		for (it = srv_it->error_page.begin(); it != srv_it->error_page.end(); it++)
			std::cout << "error_page : \t\t'" << *it << "'" << std::endl;
		for (it = srv_it->return_dir.begin(); it != srv_it->return_dir.end(); it++)
			std::cout << "return : \t\t'" << *it << "'" << std::endl;
		printLocationConfig(srv_it->location);
		srv_nb++;
		std::cout << RESET;
	}
	return ;
}

void webserv::printHttpConfig( void )
{
	std::vector<std::string>::iterator 	it;
	std::vector<t_server>::iterator		srv_it;
	std::vector<t_location>::iterator	loc_it;

	std::cout << GREEN;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "\tHTTP config" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << "autoindex : \t\t'" << _config.autoindex << "'" << std::endl;
	std::cout << "client_max_body_size :\t'" << _config.client_max_body_size << "'" << std::endl;
	std::cout << "index : \t\t'" << _config.index << "'" << std::endl;
	std::cout << "root : \t\t\t'" <<  _config.root << "'" << std::endl;
	// error pages
	for (it = _config.error_page.begin(); it != _config.error_page.end() ;it++ )
		std::cout << "error_page : \t\t'" << *it << "'" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	printServerConfig();
	std::cout << "-------------------" << std::endl;
	std::cout << RESET;
}

void webserv::tokenizeConfigFile(std::string & src)
{
	std::vector<std::string> 	token;

	std::string::iterator 		it 		= src.begin();
	std::string::iterator 		end 	= src.end();;

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
	vec_erase_empty(token);
	if (VERBOSE)
		vec_enum(token);
	parseToken(token);
	return ;
}

void webserv::listenCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<std::string>::iterator	it;

	int 	port = -1;
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		port = atoi(srv_it->listen.c_str());
		if ( port < PORT_MIN || port > PORT_MAX )
			throw std::invalid_argument(ERR_WRONG_PORT_RANGE);
		else
			listenPorts.push_back(port);
	}
	if (VERBOSE)
	{
		std::cout << "listen Ports\n";
		vec_enum(listenPorts);
	}
}

void webserv::checkParseError ( void )
{
	listenCheck();
	// errorPageCheck();
	// maxBodyCheck();
	// limitExceptCheck();
}

void webserv::parseConfigFile ( void )
{
	std::string 				config_string = "";
	std::string 				line;

	std::ifstream 				file;

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
		std::cerr << RED"Unable to open '" << _file_name << "' file.\n"RESET;
	return ;
}