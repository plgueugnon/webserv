#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "utils.hpp"

#define REQUEST_LINE_ARGS 4
#define HEADER_ARGS 14

class request
{
	public :
	// buffer filled with recv
	bool 						isBody;
	std::string 				buf;

	enum requestLineIndex
	{
		METHOD,
		PATH,
		QUERY,
		HTTP_VERSION,
	}; // requestLineIndex

	enum headerIndex
	{
		HOST,
		CONNECTION,
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
	}; // headerIndex

	std::vector<std::string> 	requestLine;
	std::vector<std::string> 	header;
	std::string 				headerbuf;
	std::string 				body;

	// constructor
	request ( void );

	void parseHeader ( void );
	void redirectBody ( void );
	void fillRequestLine ( void );
	void fillHeaders ( void );
	void printRequest ( void );
}; // request class
#endif /* REQUEST_HPP */
