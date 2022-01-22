#include "colors.hpp"
#include "headers.hpp"

void	receive_request(int client_sock)
{
	ssize_t n = 0;
	std::vector<char> buf(4096);
	t_request	request;
	int	c = 0;

	// ! Keep-alive request pas traité par défaut
	// ? A implémenter ?
	// TODO ajouter détection CRLF pour trouver le body
	if ( (n = recv(client_sock, buf.data(), 4096 - 1, 0)) > 0)
	{
		// ! Rajouter le cas des chunk request
		if (!c)
			request.method.assign(buf.data());
		else
		{
			request.headers.assign(buf.data());
			// ! Pas necessaire si on utilise find pour trouver directement les valeurs voulues
			request.headers.append("\n");
		}
		c++;
		if (VERBOSE)
			std::cout << buf.data() << std::endl;
		if (n == 0 || n == EAGAIN)
			return ;
	}
	if (n < 0)
	{
		std::cerr << YELLOW"error: failure to receive request\n"RESET;
		close(client_sock);
		return ;
	}
	manage_request(client_sock, &request);
}