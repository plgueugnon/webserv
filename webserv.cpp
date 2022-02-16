/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:40:54 by ygeslin           #+#    #+#             */
/*   Updated: 2022/02/16 11:23:43 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Aincludes.hpp"

// Parsing context flags to parse the token (AST)
#define HTTP_CONTEXT 1
#define SERVER_CONTEXT 2
#define LOCATION_CONTEXT 3

// Error exception messages
#define ERR_HTTP_MISSING "Http context is missing."
#define ERR_WRONG_AUTOINDEX "Wrong autoindex value, usage : on | off."
#define ERR_WRONG_AUTOINDEX_ARG "Autoindex, Missing semicolomn ';'."
#define ERR_NEG_BODY_SIZE "client_max_body_size : can't be negative or bigger than 1 Million"
#define ERR_BODY_SIZE_ARG "client_max_body_size, Missing semicolomn ';'."
#define ERR_INDEX_ARG "index, Missing semicolomn ';'."
#define ERR_ROOT_ARG "root, Missing semicolomn ';'."
#define ERR_LISTEN_ARG "listen, Missing semicolomn ';'."
#define ERR_LISTEN_DIGIT "listen, only digits allowed."
#define ERR_MAX_BODY_DIGIT "client_max_body_size, only digits allowed."
#define ERR_SERVER_NAME_ARG "server_name, Missing semicolomn ';'."
#define ERR_SERVER_BRACKET "server block, Missing opening bracket '{'."
#define ERR_LOCATION_BRACKET "location block, Missing opening bracket '{'."
#define ERR_LOCATION_SLASH "location route must begin by '/'."
#define ERR_ERROR_PAGE_ARG "error_page, Missing semicolomn ';'."
#define ERR_LIMIT_EXCEPT_ARG "except_limit, Missing semicolomn ';'."
#define ERR_RETURN_ARG "return arguments, code + URL."
#define ERR_RETURN_CODE "return code invalid. 300 <= code <= 308 ."
#define ERR_WRONG_DIRECTIVE "unknown directive or open brackets"
#define ERR_WRONG_PORT_RANGE "wrong port range, min 0, max 65 535."
#define ERR_WRONG_ERR_CODE "error_page code unknown."
#define ERR_WRONG_ERR_URI "multiple error_page URI is not allowed."
#define ERR_WRONG_METHOD "limit_except : wrong method."
#define ERR_BODY_MISSING "client_max_body_size argument is missing."
#define ERR_AUTOINDEX_MISSING "autoindex argument is missing."
#define ERR_INDEX_MISSING "index argument is missing."
#define ERR_ROOT_MISSING "root argument is missing."
#define ERR_LISTEN_MISSING "listen argument is missing."
#define ERR_SERVER_NAME_MISSING "server_name argument is missing."

#define BODY_MAX 1000001

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
std::vector<int> const & webserv::getListenPorts() const
{
	return listenPorts;
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
			if ( onlyDigits(it->c_str()) == false )
				throw std::invalid_argument(ERR_MAX_BODY_DIGIT);
			int 	n = atoi(it->c_str());
			if (n >= 0 && n < BODY_MAX)
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
			// debut de checking si location path se termine pas par /
			if (tmp[0] != '/' )//&& tmp[tmp.size() - 1] != '/' )
				 throw std::invalid_argument(ERR_LOCATION_SLASH);
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
			if ( onlyDigits(it->c_str()) == false )
				throw std::invalid_argument(ERR_MAX_BODY_DIGIT);
			int 	n = atoi(it->c_str());
			if (n >= 0 && n < BODY_MAX)
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
			if ( onlyDigits(it->c_str()) == false )
				throw std::invalid_argument(ERR_LISTEN_DIGIT);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_LISTEN_ARG);
			it++;
		}
		// SERVER NAME
		else if (it->compare("server_name") == 0 && flag == SERVER_CONTEXT)
		{
			it++;
			_config.server[srv_nb].server_name = (*it);
			it++;
			if (it->compare(";") != 0)
				throw std::invalid_argument(ERR_SERVER_NAME_ARG);
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
		// CLIENT MAX BODY SIZE
		else if (it->compare("client_max_body_size") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			if ( onlyDigits(it->c_str()) == false )
				throw std::invalid_argument(ERR_MAX_BODY_DIGIT);
			int 	n = atoi(it->c_str());
			if (n >= 0 && n < BODY_MAX)
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
		else if (it->compare("return") == 0 && flag == LOCATION_CONTEXT)
		{
			it++;
			while(it->compare(";") != 0)
			{
				_config.server[srv_nb].location[loc_nb].return_dir.push_back(*it);
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
	// if (VERBOSE)
	// 	printHttpConfig();
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
		for (it = loc_it->return_dir.begin(); it != loc_it->return_dir.end(); it++)
			std::cout << "return_dir : \t\t'" << *it << "'" << std::endl;
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
	// if (VERBOSE)
	// 	vec_enum(token);
	parseToken(token);
	return ;
}

// check if the listen ports are part of the ports range
void webserv::listenCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<t_location>::iterator	loc_it;

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator 	it2;

	int 	port = -1;
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		port = atoi(srv_it->listen.c_str());
		if ( port < PORT_MIN || port > PORT_MAX )
			throw std::invalid_argument(ERR_WRONG_PORT_RANGE);
		else
			listenPorts.push_back(port);
	}
	// if (VERBOSE)
	// {
	// 	std::cout << "listen Ports\n";
	// 	vec_enum(listenPorts);
	// }
}

// check if error_page codes are known error codes and if a token begin with /, we check if it's the last : syntax = code ... code /URI;
void webserv::errorReturnCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<t_location>::iterator 	loc_it;

	int 	code = -1;
	// ! check server return directive
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		if (srv_it->return_dir.size() == 0)
			;
		else if (srv_it->return_dir.size() != 2)
			throw std::invalid_argument(ERR_RETURN_ARG);
		else
		{
			code = atoi(srv_it->return_dir.front().c_str());
			if (isReturnCode(code) == false)
				throw std::invalid_argument(ERR_RETURN_CODE);
		}
			// ! check location return directive
			for (loc_it = srv_it->location.begin();
				 loc_it != srv_it->location.end();
				 loc_it++)
			{
				if (loc_it->return_dir.size() == 0)
					;
				else if (loc_it->return_dir.size() != 2)
					throw std::invalid_argument(ERR_RETURN_ARG);
				else
				{
					code = atoi(loc_it->return_dir.front().c_str());
					if (isReturnCode(code) == false)
						throw std::invalid_argument(ERR_RETURN_CODE);
				}
			}
	}
}

// check if error_page codes are known error codes and if a token begin with /, we check if it's the last : syntax = code ... code /URI;
void webserv::errorPageCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<t_location>::iterator 	loc_it;

	std::vector<std::string>::iterator	it;
	std::vector<std::string>::iterator	it2;

	int 	code = -1;
	// ! check HTTP error_pages
	for (it = _config.error_page.begin(); it != _config.error_page.end(); it++)
	{
		// if (VERBOSE)
		// 	std::cout << *it << "\n";
		// * syntax : code code [...] /URI
		if (it->compare("/") != 0)
		{
			code = atoi(it->c_str());
			if (isErrorCode(code) == false)
				throw std::invalid_argument(ERR_WRONG_ERR_CODE);
		}
		else 
		{
			// * if token commence par '/', alors token = URI
			// * verifier que c'est bien le dernier token
			if (it != (_config.error_page.end() - 1))
				throw std::invalid_argument(ERR_WRONG_ERR_URI);
		}
	}
	// ! check server error_pages
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		for (it = srv_it->error_page.begin(); it != srv_it->error_page.end(); it++)
		{
			// if (VERBOSE)
			// 	std::cout << *it << "\n";
			if (it[0][0] != '/')
			{
				code = atoi(it->c_str());
				if (isErrorCode(code) == false)
					throw std::invalid_argument(ERR_WRONG_ERR_CODE);
			}
			else
			{
				if (it != (srv_it->error_page.end() - 1))
					throw std::invalid_argument(ERR_WRONG_ERR_URI);
			}
		}
		// ! check location error_pages
		for (loc_it = srv_it->location.begin();
			 loc_it != srv_it->location.end();
			 loc_it++)
		{
			for (it2 = loc_it->error_page.begin();
				 it2 != loc_it->error_page.end();
				 it2++)
			{
				// if (VERBOSE)
				// 	std::cout << *it2 << "\n";
				if (it2[0][0] != '/')
				{
					code = atoi(it2->c_str());
					if (isErrorCode(code) == false)
						throw std::invalid_argument(ERR_WRONG_ERR_CODE);
				}
				else
				{
					if (it2 != (loc_it->error_page.end() - 1))
						throw std::invalid_argument(ERR_WRONG_ERR_URI);
				}
			}
		}
	}
}

// check if limited methode are GET POST or DELETE
void webserv::limitExceptCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<t_location>::iterator 	loc_it;

	std::vector<std::string>::iterator	it;

	// ! iterate servers
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		// ! iterate location
		for (loc_it = srv_it->location.begin();
			 loc_it != srv_it->location.end();
			 loc_it++)
		{
			for (it = loc_it->limit_except.begin();
				 it != loc_it->limit_except.end();
				 it++)
			{
				// if (VERBOSE)
				// 	std::cout << *it << "\n";
				if (	it->compare("GET") != 0 &&
						it->compare("POST") != 0 && 
						it->compare("DELETE") != 0 )
					throw std::invalid_argument(ERR_WRONG_METHOD);
			}
		}
	}
}

/*
if some server settings are missing, we fill it with http context settings
if some location settings are missing, we fill it with server context settings
*/
void webserv::fillDefaultSettings ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;
	std::vector<t_location>::iterator 	loc_it;

	std::vector<std::string>::iterator	it;

	int		srv_nb = 0;
	int		loc_nb = 0;

	// ! iterate servers
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		loc_nb = 0;
		if (srv_it->client_max_body_size == 0)
			_config.server[srv_nb].client_max_body_size = _config.client_max_body_size;
		if (srv_it->autoindex.empty() == true)
			_config.server[srv_nb].autoindex = _config.autoindex;
		if (srv_it->index.empty() == true)
			_config.server[srv_nb].index = _config.index;
		if (srv_it->root.empty() == true)
			_config.server[srv_nb].root = _config.root;
		if (srv_it->error_page.empty() == true)
			_config.server[srv_nb].error_page = _config.error_page;
		// ! iterate location
		for (loc_it = srv_it->location.begin();
			 loc_it != srv_it->location.end();
			 loc_it++)
		{
			if (loc_it->client_max_body_size == 0)
				_config.server[srv_nb].location[loc_nb].client_max_body_size =
					_config.server[srv_nb].client_max_body_size;
			if (loc_it->autoindex.empty())
				_config.server[srv_nb].location[loc_nb].autoindex =
					_config.server[srv_nb].autoindex;
			if (loc_it->index.empty())
				_config.server[srv_nb].location[loc_nb].index =
					_config.server[srv_nb].index;
			if (loc_it->root.empty())
				_config.server[srv_nb].location[loc_nb].root =
					_config.server[srv_nb].root;
			if (loc_it->error_page.empty())
				_config.server[srv_nb].location[loc_nb].error_page =
					_config.server[srv_nb].error_page;
			if (loc_it->return_dir.empty())
				_config.server[srv_nb].location[loc_nb].return_dir =
					_config.server[srv_nb].return_dir;
			loc_nb++;
		}
		srv_nb++;
	}


}

// Check if a non-optional setting is missing
void webserv::emptySettingCheck ( void )
{
	std::vector<t_server> 				srv = _config.server;

	std::vector<t_server>::iterator 	srv_it;

	std::vector<std::string>::iterator	it;

	// ! iterate servers
	for (srv_it = srv.begin(); srv_it != srv.end(); srv_it++)
	{
		if (srv_it->client_max_body_size == 0)
			throw std::invalid_argument(ERR_BODY_MISSING);
		if (srv_it->autoindex.empty() == true)
			throw std::invalid_argument(ERR_AUTOINDEX_MISSING);
		if (srv_it->index.empty() == true)
			throw std::invalid_argument(ERR_INDEX_MISSING);
		if (srv_it->root.empty() == true)
			throw std::invalid_argument(ERR_ROOT_MISSING);
		if (srv_it->server_name.empty() == true)
			throw std::invalid_argument(ERR_SERVER_NAME_MISSING);
		if (srv_it->listen.empty() == true)
			throw std::invalid_argument(ERR_LISTEN_MISSING);
	}
}

// main loop for checking parsing errors
void webserv::checkParseError ( void )
{
	listenCheck();
	errorPageCheck();
	errorReturnCheck();
	limitExceptCheck();
	fillDefaultSettings();
	emptySettingCheck();
	// if (VERBOSE)
	// 	printHttpConfig();
}

void webserv::parseConfigFile ( void )
{
	std::string 				config_string = "";
	std::string 				line;

	std::ifstream 				file;

	std::vector<std::string> 	token;

	if (_file_name.find(".conf") == std::string::npos)
		throw std::invalid_argument("Wrong extension file.");

	file.open(_file_name.c_str(), 	std::ios::in  );

	if (file.good())
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
		throw std::invalid_argument("Unable to open config file.");
	return ;
}