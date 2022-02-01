/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP


// print all nodes of a vector
void    vec_enum(std::vector<std::string> &vec);
void    vec_enum(std::vector<int> &vec);
// erase empty token in the vector
void    vec_erase_empty(std::vector<std::string> &vec);
void    print_env_c(const char ** env);

bool     is_error_code ( int  code);
bool     onlyDigits ( const char *  str);

void printLocation ( t_location *loc);


#endif /* UTILS_HPP */
