#include "colors.hpp"
#include "headers.hpp"

void	receive_request(int client_sock)
{
	ssize_t n = 0;
	std::vector<char> buf(4096);
	char	tab[256];
	t_request	request;
	// int	c = 0;

	// ! Keep-alive request pas traité par défaut
	// ? A implémenter ?
	// TODO ajouter détection CRLF pour trouver le body
// ! NB If a request contains a message-body and a Content-Length is not given, the server SHOULD respond with 400 (bad request) if it cannot determine the length of the message, or with 411 (length required)

	// ! PAS VALIDE .data = c++11
	// if ( (n = recv(client_sock, buf.data(), 4096 - 1, 0)) > 0)
	while ( (n = recv(client_sock, tab, 256 - 1, 0)) > 0)
	{
		// ! Rajouter le cas des chunk request
		// * https://fr.wikipedia.org/wiki/Chunked_transfer_encoding
		// if (!c)
		// 	// request.method.assign(buf.data());
		// 	request.method.append(tab);
		// else
		// {
		// 	request.headers.append(tab);
		// // ! Pas necessaire si on utilise find pour trouver directement les valeurs voulues
		// 	request.headers.append("\n");
		// }
		// c++;
		if (VERBOSE)
			std::cout << tab << std::endl;
		// buf.clear();
		// if (n == 0 || n == EAGAIN)
		// if ( n == EAGAIN )
		// 	return ;
		if ( n == 0 )
		{
			// close(client_sock);
			break ;
		}
		if (n < 0)
		{
			std::cerr << YELLOW"error: failure to receive request\n"RESET;
			close(client_sock);
			return ;
		}
	}
	if (VERBOSE)
		std::cout << "PUTE" << tab << std::endl;

	if (VERBOSE)
	{
		std::cout << YELLOW"Checking struct content : "RESET << request.method.c_str() << "\n";
		std::cout << YELLOW"Checking struct headers : "RESET << request.headers.c_str() << "\n";
	}
	manage_request(client_sock, &request);
}