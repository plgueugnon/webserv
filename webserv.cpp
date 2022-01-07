#include "webserv.hpp"

webserv::webserv ( void )
{ return ;}

void webserv::setFileName (std::string name)
{
	_file_name.assign(name);
}
