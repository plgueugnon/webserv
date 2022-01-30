#include "colors.hpp"
#include "utils.hpp"
#include "headers.hpp"

request::request ( void ) :
isBody(false),
buf(""), 
requestLine((REQUEST_LINE_ARGS)),
header((HEADER_ARGS)),
headerbuf(""),
body("")
{
	return ;
}

#define BUFFER_SIZE 12

void	receive_request(int client_sock)
{
	ssize_t n = 0;
	// std::vector<char> buf(4096);
	// t_request	request;
	// int	c = 0;
	// std::string 	buffer = "";
	char buffer[BUFFER_SIZE];
	request 		request;

	// ! Keep-alive request pas traité par défaut
	// ? A implémenter ?
	// TODO ajouter détection CRLF pour trouver le body
	while ( (n = recv(client_sock, &buffer, BUFFER_SIZE - 1, 0)) > 0)
	{
		buffer[n] = '\0';
		if (request.isBody == false)
		{
			request.buf += buffer;
			request.parseHeader();
		}
		else
		{
			request.buf = buffer;
			request.redirectBody();
		}


		
		// ! Rajouter le cas des chunk request
		// if (!c)
		// 	request.method.assign(buf.data());
		// else
		// {
		// 	request.headers.assign(buf.data());
		// 	// ! Pas necessaire si on utilise find pour trouver directement les valeurs voulues
		// 	request.headers.append("\n");
		// }
		// c++;
		// if (VERBOSE)
		// 	std::cout << buf.data() << std::endl;
		if (n == 0 || n == EAGAIN )
			return ;
		if (n < BUFFER_SIZE - 1)
			break;
	}
	if (n < 0)
	{
		std::cerr << YELLOW"error: failure to receive request\n"RESET;
		close(client_sock);
		return ;
	}
	request.printRequest();
	// std::cout << request.buf << std::endl;
	close(client_sock);
	// std::cout << "close socket" << std::endl;
	// manage_request(client_sock, &request);
}

// return -1 if str doesn't contain \r\n
// return the pos of the \r
int 	containsCrlf(std::string str)
{
	std::string::iterator 	it = str.begin();
	std::string::iterator 	end = str.end();
	int 					i = 0;

	for (; it != end; it++)
	{
		if (str.size() > 1)
			if (*it == '\r' && *(it - 1) == '\n')
				return (i);
		i++;
	}
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
	// + 2 to skip \r\n
	body = buf.substr(pos + 2, buf.size());
	// erase the end of the buffer to extract only headers
	buf.erase(pos - 1);
	// save requestline and header in headerBuffer
	headerbuf = buf;
	buf.clear();
	fillRequestLine();
	fillHeaders();

	return ;
}

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string>	vec;
	std::string 				token;
	unsigned long pos;
	while ( (pos = str.find (delim)) != std::string::npos)
	{  
		vec.push_back(str.substr(0, pos)); 
		str.erase(0, pos + 1);  /* erase() function store the current positon and move to next token. */   
	}  
		vec.push_back(str.substr(0, pos)); 
		return vec;
}

void request::fillRequestLine(void)
{
	// TODO add vector on stack
	// TODO if vector size > 3 -> error
	// TODO match vector with requestLine vector
	unsigned long pos = headerbuf.find('\n');
	std::string str = headerbuf.substr(0, pos);
	headerbuf.erase(0, pos + 1);  /* erase() function store the current positon and move to next token. */   
	requestLine = split(str, ' ');
	requestLine[HTTP_VERSION] = requestLine[2];

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
	vec.push_back("Content-Lenght: ");
	vec.push_back("Date: ");
	vec.push_back("Referer: ");
	vec.push_back("Transfer-Encoding: ");
	vec.push_back("User-Agent: ");
	return vec;

}

void request::fillHeaders(void)
{
	std::string 	tmp;
	std::string 	tmp2;
	std::vector<std::string> ToSearch = headerKeysToSearch();
	std::vector<std::string> buf = split(headerbuf, '\n');
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it2;
	std::vector<std::string>::iterator end = buf.end();
	std::vector<std::string>::iterator end2 = ToSearch.end();

	// vec_enum(buf);
	int 	headerIndex = 0;

	for (it = buf.begin(); it != end; it++)
	{
		tmp2 = *it;
		for (it2 = ToSearch.begin(); it2 != end2; it2++)
		{
			tmp = *it2;
			if (it->compare(0, tmp.length(), tmp) == 0)
			{
				tmp2.erase(0, tmp.length());
				header[headerIndex] = tmp2;
				// std::cout << "header :" << tmp << "; comp :" << tmp2 << "\n";
			// fill the corresponding header vector according to the matched string
			// erase from 0 to string to compare lenght
			}
			headerIndex++;
		}
		headerIndex = 0;
	}
	// initialize a vector with strings to search
	// itirate threw the vector, if compare == 0
	// erase from 0 to string to compare lenght
	// fill the corresponding header vector according to the matched string
	headerbuf.clear();
	return ;
}

void request::redirectBody(void)
{
	body += buf;
	// std::cout << "redirect body" << std::endl;
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