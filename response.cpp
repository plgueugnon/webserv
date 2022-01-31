#include "colors.hpp"
#include "headers.hpp"
#include "utils.hpp"

response::response ( void ) 
{
}


void	answer_client(int client_sock, std::string answer)
{
	if (send(client_sock, answer.c_str(), answer.size(), 0) == -1)
	{
		std::cerr << YELLOW"error: failure to send answer\n"RESET;
		close(client_sock);
		return ;
	}
	// ! Par défaut pas de connection keep-alive
	// ? A implementer ?

	if (VERBOSE)
		std::cout << GREEN"Closing connection with client\n"RESET;
	close(client_sock);
}

// TODO créer parsing complet et remplissage dynamique de la réponse à donner
// ? Problème si le statut disponible en écriture du client pas vérifié ?
void	manage_request(int client_sock, request *request, t_http config)
{
	response 	response;
	(void)config;
	std::string	answer;
	if (request->requestLine[request::METHOD].compare(0, 3, "GET") != 0)
		answer.assign("HTTP/1.1 400 Bad Request\r\n\r\n");
	else
		answer.assign("HTTP/1.1 200 OK\r\n\r\n Wesh ma gueule, bien ou bien !?");
	answer_client(client_sock, answer);
}

				// if (	it->compare("GET") != 0 &&
				// 		it->compare("POST") != 0 && 
				// 		it->compare("DELETE") != 0 )
				// 	throw std::invalid_argument(ERR_WRONG_METHOD);