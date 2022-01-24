/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ygeslin <ygeslin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 10:41:04 by ygeslin           #+#    #+#             */
/*   Updated: 2022/01/24 15:06:20 by ygeslin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <iostream>
#include "utils.hpp"
#include "httpContext.hpp"

class webserv
{
	/* public attributes */
	public :
		std::string 		_file_name;
		t_http 				_config;
		std::vector<int>	listenPorts;
		// listen 		socket;


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

		/* PARSING ERROR */
		void printHttpConfig ( void );
		void printServerConfig( void );

		void checkParseError( void );
		void listenCheck(void);
		void errorPageCheck(void);
		void maxBodyCheck(void);
		void limitExceptCheck(void);

} ; // webserv class

#endif
