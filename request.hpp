#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "utils.hpp"

#define REQUEST_LINE_ARGS 4
#define HEADER_ARGS 12

class request
{
	public :
	// buffer filled with recv
	std::string 				buf;

	enum requestLineIndex
	{
		METHOD,
		PATH,
		QUERY,
		HTTP_VERSION,
	}

	enum headerIndex
	{
		AUTHORIZATION,
		ACCEPT,
		ACCEPT_CHARSET,
		ACCEPT_ENCODING,
		ACCEPT_LANGUAGE,
		CONTENT_TYPE,
		CONTENT_LENGHT,
		DATE,
		REFERER,
		TRANSFER_ENCODING,
		USER_AGENT,
	}

	std::vector<std::string> 	requestLine;
	std::vector<std::string> 	header;
	std::string 				body;
}
#endif /* REQUEST_HPP */
