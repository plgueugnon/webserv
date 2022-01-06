#include <iostream>

int main (int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	(void)env;

	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "O  WEBSERV BY PILOU & YANNOU <3  O" << std::endl;
	std::cout << "O                                O" << std::endl;
	std::cout << "oooooooooooooooooooooooooooooooooo" << std::endl;

	if (ac == 1)
	{
		std::cout << "No argument entenred." << std::endl;
		std::cout << "Default configuration file as input." << std::endl;
	}

	return (1);
}
