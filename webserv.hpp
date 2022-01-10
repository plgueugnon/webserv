/* ************************************************************************** */
/* ygeslin                                                                    */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>

class webserv
{
	/* private attributes */
	private :
		std::string _file_name;

	/* public attributes */
	public :

		/* CONSTRUCTOR */
		webserv ( void );

		/* SETTER */
		void setFileName(std::string name);

		/* GETTER */
		std::string const & getFileName() const;

} ; // webserv class

#endif
