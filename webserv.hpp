/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include "httpContext.hpp"

class webserv
{
	/* private attributes */
	private :
		std::string _file_name;
		t_http 		_config;

	/* public attributes */
	public :

		/* CONSTRUCTOR */
		webserv ( void );

		/* SETTER */
		void setFileName(std::string name);

		/* GETTER */
		std::string const & getFileName() const;

		/* PARSING */
		void parseConfigFile ( void );
		void tokenizeConfigFile(std::string &src);
		void parseToken (std::vector<std::string> &vec);
		void printHttpConfig ( void );
		void printServerConfig( void );

} ; // webserv class

#endif
