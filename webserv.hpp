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

} ; // webserv class

#endif
