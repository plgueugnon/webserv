/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#include "Aincludes.hpp"

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

void printLocation ( t_location *loc)
{
    std::vector<std::string>::iterator it;

    std::cout << "---------------------" << std::endl;
    std::cout << " Location config " << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "path : \t\t\t'" << loc->path << "'" << std::endl;
    std::cout << "autoindex : \t\t'" << loc->autoindex << "'" << std::endl;
    std::cout << "client_max_body_size :\t'" << loc->client_max_body_size << "'" << std::endl;
    std::cout << "index : \t\t'" << loc->index << "'" << std::endl;
    std::cout << "root : \t\t\t'" << loc->root << "'" << std::endl;
    // error pages
    for (it = loc->error_page.begin(); it != loc->error_page.end(); it++)
        std::cout << "error_page : \t\t'" << *it << "'" << std::endl;
    // limit except
    for (it = loc->limit_except.begin(); it != loc->limit_except.end(); it++)
        std::cout << "limit_except : \t\t'" << *it << "'" << std::endl;
    for (it = loc->return_dir.begin(); it != loc->return_dir.end(); it++)
        std::cout << "return_dir : \t\t'" << *it << "'" << std::endl;
}