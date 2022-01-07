#include <iostream>
#include <stdlib.h>
#include "colors.h"
#include "webserv.hpp"

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
	std::cout << server.getFileName() << std::endl;

	return (1);
}
