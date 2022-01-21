/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:41:04 by ygeslin           #+#    #+#             */
/*   Updated: 2022/01/21 10:41:13 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include "utils.h"
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
