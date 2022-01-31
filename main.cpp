/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:40:49 by ygeslin           #+#    #+#             */
/*   Updated: 2022/01/31 14:50:50 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
check args
open and copy config file 
convert into token
fill struct with token
check errors "double key, unvalid value"
fill servers struct with http default settings if empty
create listen socket according to servers listen ports

wait for requests
parse requests
return response

if CGI
fill CGI env std::string
convert to char **
exec cgi


*/

#include <iostream>
#include <stdlib.h>
#include "colors.hpp"
#include "webserv.hpp"
#include "utils.hpp"

void print_welcome( void )
{
	std::cout << BOLDCYAN;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "O  WEBSERV BY PILOU & YANNOU <3  O" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << std::endl;
	std::cout << RESET;
	return ;
}

void usage_too_many_args ( void )
{
	std::cout << RED;
	std::cerr << "To many arguments provided. 1 argument max." << std::endl ;
	std::cerr << "Usage : ./webserv 'path/config_file'."<< std::endl;
	std::cout << RESET;
	exit (1);
}

void usage_one_arg ( void )
{
	std::cout << GREEN;
	std::cout << "No argument provided." << std::endl;
	std::cout << "Default configuration file will be used." << std::endl;
	std::cout << "You can edit file at 'etc/webserv.conf'." << std::endl;
	std::cout << RESET;
	return ;
}

void usage_two_args ( char * str )
{
	std::cout << GREEN;
	std::cout << "Loading '" << str << "'";
	std::cout << " as configuration file."<< std::endl;
	std::cout << RESET;
	return ;
}

int main (int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	webserv		server;

	print_welcome();

	if (ac > 2)
		usage_too_many_args();
	else
	{
		if (ac == 1)
		{
			usage_one_arg();
			server.setFileName("etc/webserv.conf");
		}
		if (ac == 2)
		{
			usage_two_args(av[1]);
			server.setFileName(av[1]);
		}
	}
	try 
	{
		server.parseConfigFile();
		server.checkParseError();
	}
	  catch (std::invalid_argument& e)
    {
		std::cerr << RED"Error : " << e.what() << std::endl << RESET;
		return -1;
	}
	// send ports number to socket vector<int>
	listener(&server);

// ! CGI env
	cgi cgi;
	if (VERBOSE)
	{
		vec_enum(cgi.env);
		cgi.convertToC();
		print_env_c(cgi.c_env);
	}
	
	

	return (1);
}
/*
1. parse cofing // gestion erreur de config
2. listener (recoit config en class)
3. receive request -> gerer parsing body + gerer chunk request
4. parsing request  -> appel cgi ? init class var cgi ? listing directory ? detection erreur / requete valide / (que parsing ici)
5. manage response -> appel cgi + stockage reponse cgi + choix du code de retour (que exec ici) -> retour a la boucle listener 
*/