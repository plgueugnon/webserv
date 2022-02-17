/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:40:49 by ygeslin           #+#    #+#             */
/*   Updated: 2022/02/17 17:52:35 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Aincludes.hpp"

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
	webserv		server_config;

	print_welcome();

	if (ac > 2)
		usage_too_many_args();
	else
	{
		if (ac == 1)
		{
			usage_one_arg();
			server_config.setFileName("etc/webserv.conf");
		}
		if (ac == 2)
		{
			usage_two_args(av[1]);
			server_config.setFileName(av[1]);
		}
	}
	try 
	{
		server_config.parseConfigFile();
		server_config.checkParseError();
	}
	  catch (std::invalid_argument& e)
    {
		std::cerr << RED"Error : " << e.what() << std::endl << RESET;
		return -1;
	}
	try
	{
		Server	listener(&server_config._config);
		listener.setup_config();
		listener.run();
	}
	catch (std::exception& e)
	{
		std::cerr << RED"ERROR CAUGHT: ";
		std::cerr << RED << e.what() << RESET << std::endl;
		return -1;
	}
	return ( 0 );
}
