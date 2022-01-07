#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>

class webserv
{
	private :
		std::string _file_name;
	public :
		webserv ( void );
		void setFileName(std::string name);
		std::string const & getFileName();

} ; // webserv class

#endif
