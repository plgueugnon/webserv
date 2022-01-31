#include "colors.hpp"
#include "headers.hpp"
#include "utils.hpp"

response::response ( void ) 
{
}

response::response (request *request, t_server config)
{
	req = request;
	conf = config;
	ret = "";
	req->printRequest();
}

void response::handleGet ( void )
{
	std::fstream file;
	std::string fileName = "";
	std::string line = "";
	std::string output = "";
	fileName += conf.root;
	fileName += req->requestLine[request::PATH] + "/" ;
	fileName += conf.index;
	// fileName = "www/pokemon/carapuce.png";

	std::cout << YELLOW"\nfilename: " << fileName << "\n"RESET;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		while (getline(file, line))
				output += line;
	}
	// std::cout << output << std::endl;
	ret += CODE_200;
	ret += "\r\n\r\n";
	ret += output;
	return;
}

void response::handleDelete ( void )
{
	return;
}

void response::handlePost ( void )
{
	return;
}

void response::parse ( void )
{
	if ((req->requestLine[request::METHOD]).compare("GET") != 0 &&
		(req->requestLine[request::METHOD]).compare("POST") != 0 &&
		(req->requestLine[request::METHOD]).compare("DELETE") != 0)
	{
		ret += CODE_400;
		ret += "\r\n\r\n";
		ret += "Method not handled my man !";
		return ;
	}
	// std::cout << RED << (req->requestLine[request::HTTP_VERSION]) <<"|"<< RESET;
	// // std::cout << RED << "HTTP/1.1" << "|"<< RESET;
	// if ((req->requestLine[request::HTTP_VERSION]).compare("HTTP/1.1") != 0)
	// {
	// 	ret += CODE_400;
	// 	ret += "\r\n\r\n";
	// 	ret += "Wrong http version man";
	// 	return ;
	// }
	if ( (req->requestLine[request::METHOD]).compare("GET") == 0 )
		handleGet();
	else if ( (req->requestLine[request::METHOD]).compare("DELETE") == 0 )
		handleDelete();
	else if ( (req->requestLine[request::METHOD]).compare("POST") == 0 )
		handlePost();
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
void	manage_request(int client_sock, request *request, t_server config)
{
	response 	response(request, config);
	std::string	answer;
	(void)config;
	response.parse();
	// if (request->requestLine[request::METHOD].compare(0, 3, "GET") != 0)
	// 	answer.assign("HTTP/1.1 400 Bad Request\r\n\r\n");
	// else
	// 	answer.assign("HTTP/1.1 200 OK\r\n\r\n Wesh ma gueule, bien ou bien !?");
	answer += response.ret;
	answer_client(client_sock, answer);
}

				// if (	it->compare("GET") != 0 &&
				// 		it->compare("POST") != 0 && 
				// 		it->compare("DELETE") != 0 )
				// 	throw std::invalid_argument(ERR_WRONG_METHOD);