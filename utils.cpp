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
#include "utils.h"

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

