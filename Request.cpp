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

	// std::cout << "POS:"<< pos << '\n';
	// std::cout << "---------------------" << std::endl;
	if (pos < 0)
		return;
	isBody = true;
	body = buf.substr(pos, buf.size());
	buf.erase(pos);
	headerbuf = buf;
	buf.clear();

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
	std::cout << "is Body : " << isBody << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "header: \n" << headerbuf << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "Body: \n" << body << std::endl;
	return ;
}