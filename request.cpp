#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "Aincludes.hpp"

request::request ( void ) :
isBody(false),
buf(""), 
headerbuf(""),
body("")
{
	std::vector<std::string> vec(request::NB_REQUEST_LINE_ARG);
	vec.resize(request::NB_REQUEST_LINE_ARG);
	requestLine = vec;
	std::vector<std::string> vec2(request::NB_HEADER_ARG);
	vec2.resize(request::NB_HEADER_ARG);
	header = vec2;

	return ;
}

// return -1 if str doesn't contain \r\n
// return the pos of the \r
int 	containsCrlf(std::string str)
{
	int pos = str.find("\r\n\r\n");
	if (pos > 0)
		return pos;
	return (-1);
}

void request::parseHeader(void)
{
	int 	pos = containsCrlf(buf);

	// if pos < 0, the buffer doesn't contain \r\n
	// so return to recv to receive the end of the headers
	if (pos < 0)
		return;
	isBody = true;
	// save the beginning of the body, saved in the buffer
	// + 4 to skip \r\n\r\n
	body = buf.substr(pos + 4, buf.size());
	buf.erase(pos);

	// erase the end of the buffer to extract only headers
	// save requestline and header in headerBuffer
	headerbuf = buf;
	buf.clear();
	fillRequestLine();
	fillHeaders();
	// eraseEndChar();
	// ! faire check erreur si version http differente de 1.1
	// ! faire check erreur si headers trop longs

	return ;
}

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string>	vec;
	std::string 				token;
	unsigned long 				pos;
	while ( (pos = str.find (delim)) != std::string::npos)
	{  
		vec.push_back(str.substr(0, pos)); 
		str.erase(0, pos + 1);
	}
	vec.push_back(str.substr(0, pos));
	return vec;
}

void request::fillRequestLine(void)
{
	// TODO gestion exception et erreur, a voir quand on saura comment et ou on implement les reponses
	// extract first line of the buffer
	unsigned long	pos = headerbuf.find("\r\n");
	std::string 	str = headerbuf.substr(0, pos);

// std::cout << RED"pos :" << pos << "\n"RESET;
	// erase first line of the buffer (request line)
	headerbuf.erase(0, pos);
	std::vector<std::string> vec = split(str, ' ');
	if (vec.size() != 3)
	// ! mettre exception a terme
		std::cerr << "Wrong arg nb in request line\n";
	requestLine[METHOD] = vec[0];
	requestLine[HTTP_VERSION] = vec[2];
	// extracting query
	vec = split(vec[1], '?');
	requestLine[PATH] = vec[0];
	if (vec.size() == 2 && vec[1][0] != '\n')
		requestLine[QUERY] = vec[1];
	if (vec.size() > 2)
	// ! mettre exception a terme
		std::cerr << RED"multiple ? in query \n"RESET;
	return ;
}

// order of the words match enum index of headers in the class
std::vector<std::string> headerKeysToSearch ( void )
{
	std::vector<std::string> vec;

	vec.push_back("Host: ");
	vec.push_back("Connection: ");
	vec.push_back("Authorization: ");
	vec.push_back("Accept: ");
	vec.push_back("Accept-Charset: ");
	vec.push_back("Accept-Encoding: ");
	vec.push_back("Accept-Language: ");
	vec.push_back("Content-Type: ");
	vec.push_back("Content-Length: ");
	vec.push_back("Date: ");
	vec.push_back("Referer: ");
	vec.push_back("Transfer-Encoding: ");
	vec.push_back("User-Agent: ");
	return vec;

}

void request::fillHeaders(void)
{
	// vector with the key string to search in the headers
	std::vector<std::string> toSearch 	= headerKeysToSearch(); 
	// vector with one vector node for one request header line
	std::vector<std::string> buf 		= split(headerbuf, '\n');
	// vec_enum(buf);
	
	// iterator on request headers lines
	std::vector<std::string>::iterator requestHeaders;
	// iterator on key headers strings to search
	std::vector<std::string>::iterator headerToSearch;

	int 	headerIndex = 0;

	for (requestHeaders = buf.begin(); requestHeaders != buf.end(); requestHeaders++)
	{
		for (headerToSearch = toSearch.begin(); headerToSearch != toSearch.end(); headerToSearch++)
		{
			if (requestHeaders->compare(0, headerToSearch->length(), *headerToSearch) == 0)
			{
				requestHeaders->erase(0, headerToSearch->length());
				header[headerIndex] = *requestHeaders;
			}
			headerIndex++;
		}
		headerIndex = 0;
	}
	headerbuf.clear();
	return ;
}

void request::eraseEndChar(void)
{
	std::vector<std::string>::iterator it = header.begin();

	for (; it != header.end(); it++)
	{
		if (it->back() == '\r')
			it->erase(it->end() - 1);
	}
	for (it = requestLine.begin(); it != requestLine.end(); it++)
	{
		if (it->back() == '\r')
			it->erase(it->end() - 1);
	}
}

void request::redirectBody(void)
{
	body += buf;
	return ;
}

void request::printRequest(void)
{
	std::vector<std::string> ToSearch = headerKeysToSearch();

	std::cout << "is Body : " << isBody << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "header: \n" ;
	for (size_t i = 0; i < HEADER_ARGS - 1; i++)
		std::cout << i << "- " << ToSearch[i] << header[i] << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "Body: \n" << body << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "method :" << requestLine[METHOD] << std::endl;
	std::cout << "path :" << requestLine[PATH] << std::endl;
	std::cout << "query :" << requestLine[QUERY] << std::endl;
	std::cout << "http version :" << requestLine[HTTP_VERSION] << std::endl;
	return ;
}
#endif // __REQUEST_H__