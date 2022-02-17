/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 17:46:47 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/17 17:46:52 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP


// print all nodes of a vector
void    vec_enum(std::vector<std::string> &vec);
void    vec_enum(std::vector<int> &vec);
// erase empty token in the vector
void    vec_erase_empty(std::vector<std::string> &vec);
void    print_env_c(char ** env);

bool     isErrorCode ( int  code);
bool     isReturnCode ( int  code);
bool     onlyDigits ( const char *  str);

void printLocation ( t_location *loc);


#endif /* UTILS_HPP */
