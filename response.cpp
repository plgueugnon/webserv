#include "Aincludes.hpp"

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
/*
 check if the request path match a location block
 if the request path match a location block, 
 check if there is a return directive
	return the code
 check if the method is allowed in this location block
 change root and index and autoindex
 if the path is a directory begin and finish with /
 return auto index
 if not, not found
 check if there is error_pages for this location
 */

// filename = root + request path + index
void response::handleGet ( t_location *loc )
{
	std::fstream file;
	std::string fileName = "";
	std::string line = "";
	std::string output = "";

	if (loc)
	{
		fileName += loc->root;
		fileName += req->requestLine[request::PATH];
		if (req->requestLine[request::PATH].back() == '/')
			fileName += conf.index;
	}
	else 
	{
		fileName += conf.root;
		fileName += req->requestLine[request::PATH];
		if (req->requestLine[request::PATH].back() == '/')
			fileName += conf.index;
	}
		// std::cout << RED<< req->requestLine[request::PATH].back();
		// std::cout << "----\n"RESET;
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
	std::vector<t_location>::iterator	loc_it;

	t_location 							tmp;

	if ((req->requestLine[request::METHOD]).compare("GET") != 0 &&
		(req->requestLine[request::METHOD]).compare("POST") != 0 &&
		(req->requestLine[request::METHOD]).compare("DELETE") != 0)
	{
		ret += CODE_400;
		ret += "\r\n\r\n";
		ret += "Method not handled my man !";
		return ;
	}
	// find location path (from server config) that match request path
	for (loc_it = conf.location.begin(); loc_it != conf.location.end(); loc_it++)
	{
		if ( req->requestLine[request::PATH].compare(loc_it->path) == 0 )
			tmp = *loc_it;
	}
	// printLocationConfig
	printLocation(&tmp);

	if ( (req->requestLine[request::METHOD]).compare("GET") == 0 )
		handleGet(&tmp);
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

	// if (VERBOSE)
	// 	std::cout << GREEN"Closing connection with client\n"RESET;
	// close(client_sock);
}

// TODO créer parsing complet et remplissage dynamique de la réponse à donner
// ? Problème si le statut disponible en écriture du client pas vérifié ?
void	manage_request(int client_sock, request *request, t_server config)
{
	response 	response(request, config);
	std::string	answer = "";
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

// TODO 1. Check si Bad Request = check method, puis store path et check protocol
// TODO 2. Check header validity et update cgi env
// TODO 2bis pasrse n store request uri
// TODO 3ter si POST method -> check transfer method (MIME or HTML form)
// TODO 3. go to response

// ! Response
// TODO 1. check selon method -> GET -> file/path exist ? -> DELETE = idem ? et POST -> A voir
// TODO 2. Check si appel cgi et storage contenu renvoyé
// TODO 3. Assemblage reponse (entete + body) et send
