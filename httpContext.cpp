#include "webserv.hpp"
#include "httpContext.hpp"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void httpContext::setHttpAutoindex (std::string const & rhs)
{
	_autoindex = rhs;
}

std::string const &httpContext::getHttpAutoindex( void )
{
	return _autoindex;
}