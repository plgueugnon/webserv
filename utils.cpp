/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

// System headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

// Custom headers
#include "utils.hpp"

// print all nodes of a vector
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

void    vec_enum(std::vector<int> &vec)
{
    std::vector<int>::iterator it;
    std::vector<int>::iterator it2;

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

// erase empty token in the vector
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

void    print_env_c(const char ** env)
{
    int i = 0;
    std::cout << "-------------------" << std::endl;
    while (env[i - 1])
    {
        printf("%i : %s\n", i, env[i]);
        i++;
    }
    std::cout << "-------------------" << std::endl;
    return ;
}

bool     is_error_code ( int  code)
{
    if (code >= 400 && code <= 418)
        return (true);
    else if (code >= 421 && code <= 429)
        return (true);
    else if (code == 431)
        return (true);
    else if (code == 451)
        return (true);
    else if (code >= 500 && code <= 508)
        return (true);
    else if (code >= 510 && code <= 511)
        return (true);
    else
        return (false);
}

bool     onlyDigits ( const char *  str)
{
    int i = 0;
    while (str[i])
    {
        if (!std::isdigit(str[i]))
            return false;
        i++;
    }
    return true;
}