/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

// System headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

// Custom headers
#include "webserv.hpp"
#include "colors.h"
#include "httpContext.hpp"

// print all nodes of a vector
void    vec_enum(std::vector<std::string> &vec);
// erase empty token in the vector
void    vec_erase_empty(std::vector<std::string> &vec);


#endif
