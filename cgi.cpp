/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgueugno <pgueugno@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 17:53:43 by pgueugno          #+#    #+#             */
/*   Updated: 2022/02/17 17:53:47 by pgueugno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Aincludes.hpp"

cgi::cgi (void)
{

	env.clear();
	env.push_back("SERVER_SOFTWARE=nginx/1.21.5");
	env.push_back("SERVER_NAME=");
	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env.push_back("SERVER_PORT=");
	env.push_back("REQUEST_METHOD=");
	env.push_back("PATH_INFO=");
	env.push_back("PATH_TRANSLATED=");
	env.push_back("SCRIPT_NAME=");
	env.push_back("QUERY_STRING=");
	env.push_back("REMOTE_HOST=");
	env.push_back("REMOTE_ADDR=");
	env.push_back("AUTH_TYPE=");
	env.push_back("REMOTE_USER=");
	env.push_back("REMOTE_IDENT=");
	env.push_back("CONTENT_TYPE=");
	env.push_back("CONTENT_LENGTH=");
	env.push_back("HTTP_ACCEPT=");
	env.push_back("HTTP_ACCEPT_LANGUAGE=");
	env.push_back("HTTP_USER_AGENT=");
	env.push_back("HTTP_COOKIE=");
	env.push_back("HTTP_REFERER=");
	// security of php-cgi redirect status 200
	env.push_back("REDIRECT_STATUS=200");
	env.push_back("SCRIPT_FILENAME=");
	// tmp dir where the files will be uploaded
	env.push_back("TMP_DIR=");
	c_env[NB_CGI_VAR] = 0;
}

void cgi::convertToC ( void )
{
	std::vector<std::string>::iterator it;
	int i = 0;
	for (it = env.begin() ; it != env.end(); it++)
	{
		c_env[i] = (char*)env[i].c_str();
		c_env[i][(env[i].size())] = '\0';
		i++;
	}
}