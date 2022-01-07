#include "webserv.hpp"

webserv::webserv ( void )
{ return ;}

void webserv::setFileName (std::string name)
{
	_file_name.assign(name);
}

std::string const & webserv::getFileName ( void ) const
{
	return _file_name ;
}
