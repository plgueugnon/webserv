#include <iostream>
#include <stdlib.h>
#include "colors.h"
#include "webserv.hpp"


int main (int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	webserv		server;

	std::cout << BOLDCYAN;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "O  WEBSERV BY PILOU & YANNOU <3  O" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << std::endl;
	std::cout << RESET;

	if (ac > 2)
	{
		std::cout << RED;
		std::cerr << "To many arguments provided. 1 argument max." << std::endl ;
		std::cerr << "Usage : ./webserv 'path/config_file'."<< std::endl;
		std::cout << RESET;
		return (1);
	}
	else
	{
		if (ac == 1)
		{
			std::cout << "No argument provided." << std::endl;
			std::cout << "Default configuration file will be used." << std::endl;
			std::cout << "You can edit file at 'etc/webserv.conf'." << std::endl;
			server.setFileName("etc/webserv.conf");
		}
		if (ac == 2)
		{
			std::cout << "Loading '" << av[1] << "'";
			std::cout << " as configuration file."<< std::endl;
			server.setFileName(av[1]);
		}

	}

	return (1);
}
