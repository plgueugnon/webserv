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

/*
 * LIST OF CONTEXTS
 *	- MAIN
 *	- events
 *	- http
 *	- upstream
 *	- mail
 *
 * LIST OF DIRECTIVES
 * -----------------------------------------------------------------------------
 * Syntax:	autoindex on | off;
 * -----------------------------------------------------------------------------
 * Default:	
 * autoindex off;
 * Context:	http, server, location
 * Enables or disables the directory listing output.
 * -----------------------------------------------------------------------------
 *  Syntax:	client_max_body_size size;
 * -----------------------------------------------------------------------------
 * Default:	client_max_body_size 1m;
 * Context:	http, server, location
 * Sets the maximum allowed size of the client request body. If the size in a
 * request exceeds the configured value, the 413 (Request Entity Too Large) error 
 * is returned to the client. Please be aware that browsers cannot correctly 
 * display this error. Setting size to 0 disables checking of client request body 
 * size.
 * -----------------------------------------------------------------------------
 * Syntax:	events { ... }
 * -----------------------------------------------------------------------------
 * Default:	—
 * Context:	main
 * Provides the configuration file context in which the directives that affect 
 * connection processing are specified.
 *

 */

void webserv::parseConfigFile ( void )
{
	return ;
}
