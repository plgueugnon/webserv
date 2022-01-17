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

void httpContext::setHttpClientMaxBodySize (size_t const & rhs)
{
	_client_max_body_size = rhs;
}

void httpContext::setHttpIndex (std::string const & rhs)
{
	_index = rhs;
}

void httpContext::setHttpRoot (std::string const & rhs)
{
	_root = rhs;
}

void httpContext::setHttpErrorPage (std::string const & rhs)
{
	_error_page.push_back(rhs);
}

void httpContext::printHttpConfig( void )
{
	std::cout << "-------------------" << std::endl;
	std::cout << "HTTP config" << std::endl;
	std::cout << "-------------------" << std::endl;
	std::cout << "autoindex : '" << getHttpAutoindex() << "'" << std::endl;
	std::cout << "client_max_body_size : '" << getHttpClientMaxBodySize() << "'" << std::endl;
	std::cout << "index : '" << getHttpIndex() << "'" << std::endl;
	std::cout << "root : '" << getHttpRoot() << "'" << std::endl;
	std::cout << "-------------------" << std::endl;
}

std::string const &httpContext::getHttpAutoindex( void )
{
	return _autoindex;
}

size_t const &httpContext::getHttpClientMaxBodySize( void )
{
	return _client_max_body_size;
}

std::string const &httpContext::getHttpIndex( void )
{
	return _index;
}

std::string const &httpContext::getHttpRoot( void )
{
	return _root;
}

std::vector<std::string> &httpContext::getHttpErrorPage( void )
{
	return _error_page;
}
