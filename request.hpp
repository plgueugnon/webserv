#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Aincludes.hpp"

#define REQUEST_LINE_ARGS 4
#define HEADER_ARGS 14

class request
{
	public :
	// buffer filled with recv
	bool 						isBody;
	bool 						BodyReady;
	std::string 				buf;
	size_t 						headerSize;

	enum requestLineIndex
	{
		METHOD,
		PATH,
		QUERY,
		HTTP_VERSION,
		NB_REQUEST_LINE_ARG,
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
		CONTENT_LENGTH,
		DATE,
		REFERER,
		TRANSFER_ENCODING,
		USER_AGENT,
		NB_HEADER_ARG,
	}; // headerIndex

	std::vector<std::string> 	requestLine;
	std::vector<std::string> 	header;
	std::string 				headerbuf;
	std::string 				body;

	// constructor
	request ( void );
	~request ( void );

	void parseHeader ( void );
	void redirectBody ( void );
	void fillRequestLine ( void );
	void fillHeaders ( void );
	void eraseEndChar ( void );
	void printRequest ( void );
}; // request class
#endif /* REQUEST_HPP */
